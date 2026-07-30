param(
    [Parameter(Mandatory = $true)]
    [string]$PrNumber,

    # Adjust to whatever model is actually pulled on this machine (`ollama list`).
    [string]$Model = "qwen2.5-coder:14b",

    [string]$OllamaUrl = "http://localhost:11434/api/generate"
)

$ErrorActionPreference = "Stop"

$diff = gh pr diff $PrNumber
if (-not $diff) {
    throw "Could not fetch diff for PR #$PrNumber (check GH_TOKEN / gh auth)."
}

$systemPrompt = @"
You are the programming lead reviewing a pull request for a small C++ game/engine project (CMake, SDL2, glad, glm).
Review the diff below. Judge correctness, obvious bugs, resource/ownership issues (RAII, leaks), and whether it matches
the project's coding guidelines (PascalCase types, camelCase functions, camelCase_ members, no premature abstraction).

Hard rules to avoid false positives:
- Only flag something if you can quote the exact line(s) from the diff below that show the problem. Do not comment on
  code you cannot see in the diff.
- Before flagging a missing keyword, modifier, or pattern (e.g. "should be virtual", "should be const"), re-check the
  quoted line yourself to confirm it is actually missing there - do not assume based on general best practice alone.
- Do not claim a standard library or third-party function (SDL, glad, glm, STL) behaves a certain way (e.g. has a
  return value, throws, etc.) unless you are certain of its real signature. If unsure, do not make the claim.
- If you are not confident an issue is real, omit it rather than guessing.

Reply in this exact format:

VERDICT: APPROVE or REQUEST_CHANGES
COMMENTS:
- one bullet per concrete issue. Format: "file:line - issue. Quoted code: `<exact line from diff>`"
- if there are no issues, write "None."
"@

$fullPrompt = "$systemPrompt`n`nDIFF:`n$diff"

$body = @{
    model   = $Model
    prompt  = $fullPrompt
    stream  = $false
    options = @{
        temperature = 0.15
        top_p       = 0.9
    }
} | ConvertTo-Json -Depth 5

$response = Invoke-RestMethod -Uri $OllamaUrl -Method Post -Body $body -ContentType "application/json"
$reviewText = $response.response

Write-Output $reviewText

$commentBody = "**Automated review (Ollama / $Model)**`n`n$reviewText"
gh pr comment $PrNumber --body $commentBody

if ($reviewText -match "VERDICT:\s*REQUEST_CHANGES") {
    Write-Output "Review requested changes."
    exit 1
}
