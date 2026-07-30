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

Reply in this exact format:

VERDICT: APPROVE or REQUEST_CHANGES
COMMENTS:
- one bullet per concrete issue, with file/line if visible in the diff
- if there are no issues, write "None."
"@

$fullPrompt = "$systemPrompt`n`nDIFF:`n$diff"

$body = @{
    model  = $Model
    prompt = $fullPrompt
    stream = $false
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
