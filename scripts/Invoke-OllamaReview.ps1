param(
    [Parameter(Mandatory = $true)]
    [string]$PrNumber,

    # Adjust to whatever model is actually pulled on this machine (`ollama list`).
    [string]$Model = "qwen2.5-coder:14b",

    # Model used for the second-pass verification of each candidate issue.
    # Deliberately a different model from $Model by default (cross-model
    # verification) rather than asking a model to mark its own homework -
    # asking qwen to re-check qwen's own claims tended to also reject valid
    # style findings, not just hallucinations.
    [string]$VerifyModel = "deepseek-coder-v2:16b",

    [string]$OllamaUrl = "http://localhost:11434/api/generate"
)

$ErrorActionPreference = "Stop"

function Invoke-Ollama {
    param(
        [string]$Prompt,
        [string]$ModelName,
        [double]$Temperature = 0.15,
        [switch]$Json
    )

    $body = @{
        model   = $ModelName
        prompt  = $Prompt
        stream  = $false
        options = @{
            temperature = $Temperature
            top_p       = 0.9
        }
    }
    if ($Json) {
        $body["format"] = "json"
    }

    $response = Invoke-RestMethod -Uri $OllamaUrl -Method Post -Body ($body | ConvertTo-Json -Depth 5) -ContentType "application/json"
    return $response.response
}

function Get-NormalizedText {
    param([string]$Text)
    # Collapse whitespace and strip diff line markers (+/-/space prefix) so a
    # quoted snippet can be matched against the diff regardless of indentation
    # or which side of the diff it came from.
    $lines = $Text -split "`n" | ForEach-Object { $_ -replace '^[+\- ]', '' }
    return (($lines -join ' ') -replace '\s+', ' ').Trim()
}

function Get-NormalizedCategory {
    param([string]$Category)
    # The model's JSON output is only guaranteed to be syntactically valid
    # JSON (via Ollama's format=json), not schema-conformant - this maps
    # synonyms/typos onto the three canonical categories. Unknown/missing
    # defaults to "correctness" (the strictest bar) so a categorization
    # slip can never accidentally soften a real bug into a non-blocking one.
    switch (([string]$Category).Trim().ToLowerInvariant()) {
        "correctness"   { "correctness" }
        "bug"           { "correctness" }
        "best-practice" { "best-practice" }
        "best practice" { "best-practice" }
        "modernization" { "best-practice" }
        "style"         { "style" }
        "naming"        { "style" }
        default         { "correctness" }
    }
}

function Format-CommentGroup {
    param(
        [string]$Heading,
        [array]$Items
    )
    if ($Items.Count -eq 0) { return $null }
    $body = ($Items | ForEach-Object {
        $suggestionLine = if ($_.suggestion) { "`n  Suggestion: $($_.suggestion)" } else { "" }
        "- $($_.file):$($_.line) - $($_.issue)`n  Quoted code: $($_.quoted_code)$suggestionLine"
    }) -join "`n"
    return "$Heading`n$body"
}

$diff = gh pr diff $PrNumber
if (-not $diff) {
    throw "Could not fetch diff for PR #$PrNumber (check GH_TOKEN / gh auth)."
}
$normalizedDiff = Get-NormalizedText $diff

$guidelinesPath = Join-Path $PSScriptRoot "..\docs\CodingGuidelines.md"
$codingGuidelines = if (Test-Path $guidelinesPath) {
    Get-Content -Path $guidelinesPath -Raw
} else {
    Write-Warning "Could not find CodingGuidelines.md at $guidelinesPath - proceeding without it."
    ""
}

# ---------------------------------------------------------------------------
# Stage 1: generate candidate issues as structured JSON.
# ---------------------------------------------------------------------------
$generatePrompt = @"
You are the programming lead reviewing a pull request for a small C++ game/engine project (CMake, SDL2, glad, glm).

You have two equally important jobs, not one:
1. CORRECTNESS: find actual bugs - things that are factually wrong, will misbehave, crash, leak, or violate the
   language/API contract.
2. MODERNIZATION: push this codebase toward modern, idiomatic C++ and this project's own written best practices,
   even when the existing code compiles and "works". A reviewer who only ever says "looks fine" because nothing is
   technically broken is not doing the job. Actively look, in every changed file, for missed opportunities in at
   least these areas:
   - Resource ownership: raw new/delete, or a raw pointer used to own a resource, instead of a smart pointer (RAII).
   - const-correctness: methods that don't modify state but aren't marked const; values that are never reassigned
     but declared non-const.
   - References vs. raw pointers: a raw pointer parameter or member that is never null and never reseated should
     usually be a reference instead.
   - Modern initialization and idiom: C-style casts, NULL or 0 used for pointers instead of nullptr, missing
     member-initializer lists, manual loops that a standard algorithm expresses more clearly.
   - Virtual/override hygiene: overriding methods not marked override, base classes with virtual dispatch but a
     non-virtual destructor, classes not meant to be subclassed not marked final.

This project has a written coding guideline. Treat it as the concrete, authoritative definition of "best practice"
for this repo, and prefer it over generic C++ folklore you may recall:

--- START docs/CodingGuidelines.md ---
$codingGuidelines
--- END docs/CodingGuidelines.md ---

On "no premature abstraction": this project's design document says its generalization rule is to only abstract
when at least two concrete use cases exist. That rule is about not inventing new class hierarchies, indirection
layers, plugin or config systems, or generic frameworks the project doesn't need yet. It does NOT mean "skip basic
resource-ownership correctness". Replacing a manual new/delete with a smart pointer, or adding a missing const,
does not add a new abstraction or a new use case - it correctly applies a tool the standard library already
provides, for a single responsibility the code already has. That is baseline hygiene, appropriate at any project
maturity level including an early bootstrap PR, and you should suggest it. Only hold back on a suggestion if it
would introduce a genuinely new type, hierarchy, or piece of configurability with no second concrete use case yet.

Hard rules that apply to EVERY finding, correctness or modernization alike - these are what keep you from being
wrong, not what keep you from commenting:
- Only flag something if you can quote the exact line(s) from the diff below that show it. Do not comment on code
  you cannot see in the diff.
- Before flagging a missing keyword, modifier, or pattern (e.g. "should be virtual", "should be const", "should be
  a smart pointer"), re-check the quoted line yourself, character by character, to confirm it is actually missing -
  do not assume based on general best practice alone.
- Do not claim a standard library or third-party function (SDL, glad, glm, STL) behaves a certain way (e.g. has a
  return value, throws, etc.) unless you are certain of its real signature. If unsure, do not make the claim.
- A modernization suggestion must fit the code that is actually there. Do not suggest a library, header, or
  language feature the project doesn't already use, unless it is a standard-library, drop-in improvement.
- Only raise a modernization suggestion if it is a clear, unambiguous improvement an experienced C++ reviewer
  would actually leave as a comment - not every conceivable nitpick.
- If you are not confident an issue or suggestion is both real and applicable, omit it rather than guessing.

Classify every finding into exactly one category:
- "correctness": a real bug - wrong behavior, undefined behavior, a leak that actually leaks, a broken invariant,
  or a misuse of an API's real documented contract.
- "best-practice": the code works, but a more modern, idiomatic, or safer construct clearly fits better (ownership,
  const-correctness, references vs. pointers, override/final, nullptr, initialization, algorithms over manual
  loops).
- "style": a naming, formatting, or comment convention from the guideline above.

Output ONLY a JSON array, no prose, no markdown code fences. Each element:
{
  "file": "path/to/file",
  "line": <line number as an integer, or null if not clear>,
  "category": "correctness" | "best-practice" | "style",
  "quoted_code": "the exact line(s) from the diff that show the problem, copied verbatim",
  "issue": "one sentence: what is wrong (correctness) or what could be improved (best-practice / style)",
  "suggestion": "one sentence: the concrete change to make"
}
If there are no issues, output exactly: []

DIFF:
$diff
"@

$rawCandidates = Invoke-Ollama -Prompt $generatePrompt -ModelName $Model -Temperature 0.15 -Json

try {
    $candidates = $rawCandidates | ConvertFrom-Json
} catch {
    Write-Warning "Could not parse candidate JSON from $Model, treating as no issues found. Raw output:`n$rawCandidates"
    $candidates = @()
}
if ($null -eq $candidates) { $candidates = @() }
# ConvertFrom-Json returns a single object (not an array) when the JSON array has one element.
if ($candidates -isnot [System.Collections.IEnumerable] -or $candidates -is [string]) {
    $candidates = @($candidates)
}

foreach ($c in $candidates) {
    $c | Add-Member -NotePropertyName category -NotePropertyValue (Get-NormalizedCategory $c.category) -Force
}

$catSummary = if ($candidates.Count -gt 0) {
    ($candidates | Group-Object category | ForEach-Object { "$($_.Count) $($_.Name)" }) -join ", "
} else {
    "none"
}
Write-Output "Stage 1: $($candidates.Count) candidate issue(s) from $Model ($catSummary)."

# ---------------------------------------------------------------------------
# Stage 2: deterministic check - the quoted code must actually appear in the diff.
# ---------------------------------------------------------------------------
$quoteFiltered = @()
foreach ($c in $candidates) {
    $quoted = Get-NormalizedText ([string]$c.quoted_code)
    if ($quoted -and $normalizedDiff.Contains($quoted)) {
        $quoteFiltered += $c
    } else {
        Write-Output "Dropped [$($c.category)] (quoted code not found verbatim in diff): $($c.file):$($c.line) - $($c.issue)"
    }
}

Write-Output "Stage 2: $($quoteFiltered.Count) candidate(s) survived the quote check."

# ---------------------------------------------------------------------------
# Stage 3: re-verify each surviving candidate against the diff in a separate,
# focused call so the model can't lean on its own earlier (possibly wrong)
# reasoning. Skeptical, low-temperature, single-issue-at-a-time. The bar is
# branched by category: correctness claims must be provably true; best-
# practice/style suggestions only need to be truthful about the code and
# genuinely applicable - "the code already works" is not grounds to reject
# a suggestion that it could be better.
# ---------------------------------------------------------------------------
$correctnessBar = @"
CORRECTNESS bar - be skeptical, since most wrong review comments are wrong because the code already does what is
being asked for, or the claimed API behavior is incorrect. Mark VALID only if the quoted code, exactly as shown,
truly and unambiguously exhibits the claimed bug right now, with no reasonable alternative reading. Mark INVALID if
the claim depends on standard-library or third-party (SDL, glad, glm, STL) behavior you are not fully certain of.
"@

$bestPracticeBar = @"
BEST-PRACTICE / STYLE bar - this is a suggestion that the code could be better, not a claim that it is broken.
Judge only two things: (1) is the quoted code real and does it actually do what the claim says it does, and (2) is
the suggested alternative a real, sensible, applicable fit for this exact code. Do NOT mark INVALID just because
the existing code compiles, runs, doesn't crash, doesn't currently leak, or is "standard" or "common" C++ practice
- that is true of most pre-modern C++ code and is exactly the situation this kind of suggestion exists to improve.
"It works" is not a rebuttal to "it could be more idiomatic". Only mark INVALID if the quoted code is misquoted or
does not match the diff, the factual claim about what it currently does is wrong, or the suggested alternative
genuinely would not fit or would not be an improvement here.
"@

$verified = @()
foreach ($c in $quoteFiltered) {
    $verificationBar = if ($c.category -eq "correctness") { $correctnessBar } else { $bestPracticeBar }

    $verifyPrompt = @"
You are fact-checking a single code review comment against a diff.

DIFF:
$diff

CLAIM TO CHECK:
File: $($c.file)
Line: $($c.line)
Category: $($c.category)
Quoted code: $($c.quoted_code)
Claimed issue: $($c.issue)
Suggested fix: $($c.suggestion)

Check in this order:

STEP 1 (applies to every category): self-contradiction check. If the claimed issue says something is missing or
absent (for example "not declared virtual", "missing const", "no override", "not using RAII"), look at the quoted
code above and check whether that exact thing is already present in it. If the quoted code already contains what
the claim says is missing, the claim contradicts its own evidence - mark INVALID immediately and skip step 2.

STEP 2: apply the bar below for this claim's category.

$verificationBar

Reply with exactly two lines:
VERDICT: VALID or INVALID
REASON: one sentence
"@

    $verifyResult = Invoke-Ollama -Prompt $verifyPrompt -ModelName $VerifyModel -Temperature 0.0

    if ($verifyResult -match "VERDICT:\s*VALID") {
        $verified += $c
    } else {
        Write-Output "Dropped [$($c.category)] (failed verification): $($c.file):$($c.line) - $($c.issue)"
        if ($verifyResult -match "REASON:\s*(.+)") {
            Write-Output "  Reason: $($Matches[1])"
        }
    }
}

Write-Output "Stage 3: $($verified.Count) candidate(s) survived verification."

# ---------------------------------------------------------------------------
# Compose the final review from whatever survived both checks, grouped by
# category. Only "correctness" findings block the check (exit 1) -
# best-practice/style findings are still posted in full, just informational.
# ---------------------------------------------------------------------------
if ($verified.Count -eq 0) {
    $verdict = "APPROVE"
    $commentsText = "None."
} else {
    $bugs          = @($verified | Where-Object { $_.category -eq "correctness" })
    $bestPractices = @($verified | Where-Object { $_.category -eq "best-practice" })
    $styleNotes    = @($verified | Where-Object { $_.category -eq "style" })

    $sections = @(
        Format-CommentGroup -Heading "**Bugs / correctness:**" -Items $bugs
        Format-CommentGroup -Heading "**Best-practice / modernization suggestions:**" -Items $bestPractices
        Format-CommentGroup -Heading "**Style:**" -Items $styleNotes
    ) | Where-Object { $_ }

    $commentsText = $sections -join "`n`n"
    $verdict = if ($bugs.Count -gt 0) { "REQUEST_CHANGES" } else { "APPROVE" }
}

$verdictNote = if ($verdict -eq "APPROVE" -and $verified.Count -gt 0) {
    " (no blocking bugs; suggestions below are informational)"
} else {
    ""
}

$reviewText = "VERDICT: $verdict$verdictNote`n`nCOMMENTS:`n$commentsText"
Write-Output $reviewText

$modelLabel = if ($VerifyModel -eq $Model) { $Model } else { "$Model, verified by $VerifyModel" }
$commentBody = "**Automated review (Ollama / $modelLabel)**`n`n$reviewText"

# Written to a temp file and posted via --body-file rather than passed as a
# command-line argument: the review text can contain quotes/backticks (e.g.
# quoted source lines), which native argument parsing on Windows will
# otherwise split into multiple args and make `gh pr comment` fail.
$commentFile = New-TemporaryFile
Set-Content -Path $commentFile -Value $commentBody -NoNewline
try {
    gh pr comment $PrNumber --body-file $commentFile
    if ($LASTEXITCODE -ne 0) {
        throw "gh pr comment failed with exit code $LASTEXITCODE"
    }
} finally {
    Remove-Item $commentFile -ErrorAction SilentlyContinue
}

if ($verdict -eq "REQUEST_CHANGES") {
    Write-Output "Review requested changes."
    exit 1
}
