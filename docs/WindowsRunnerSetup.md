# Setup: Windows self-hosted GitHub Actions runner for AI code review

This machine hosts Ollama and will run the `AI Code Review (Ollama)` GitHub
Actions workflow (`.github/workflows/ai-code-review.yml`) as a self-hosted
runner. This doc is the full setup — follow it top to bottom.

Repo: `dasPanjo/Quincunx` (adjust if the remote name/owner differs).

## 1. Prerequisites

- [ ] Ollama installed and running (`ollama serve`, or already running as a
      background service). Confirm with:
  ```powershell
  ollama list
  ```
  Note the exact model name/tag shown (e.g. `qwen2.5-coder:14b`) — you'll need
  it in step 4.
- [ ] GitHub CLI installed: `winget install --id GitHub.cli`
- [ ] Git installed and this repo cloned locally.

## 2. Register the self-hosted runner

1. On GitHub: repo → **Settings → Actions → Runners → New self-hosted runner**.
2. Choose **Windows**, architecture **x64**.
3. GitHub shows a set of PowerShell commands with a unique registration token
   — copy and run them exactly as shown, in a fresh folder (e.g.
   `C:\actions-runner`). It looks like:
   ```powershell
   mkdir actions-runner; cd actions-runner
   Invoke-WebRequest -Uri <download-url> -OutFile actions-runner-win-x64.zip
   Expand-Archive -Path actions-runner-win-x64.zip -DestinationPath .
   ./config.cmd --url https://github.com/dasPanjo/Quincunx --token <TOKEN>
   ```
   (Use the exact URL/token GitHub gives you — they're per-repo and
   single-use, don't reuse the ones written here.)
4. When `config.cmd` asks for labels, you can accept the default (it'll be
   tagged `self-hosted`, `Windows`, `X64` — matches `runs-on: self-hosted` in
   the workflow).

## 3. Install the runner as a Windows service

So it starts automatically and survives reboots/logouts:

```powershell
cd C:\actions-runner
./svc.cmd install
./svc.cmd start
```

Verify it shows up as **Idle** under repo → Settings → Actions → Runners.

## 4. Set the correct Ollama model in the review script

Open `scripts/Invoke-OllamaReview.ps1` in this repo and check the default
value of `-Model` matches what `ollama list` showed in step 1. Edit if
needed, commit, and push:

```powershell
git add scripts/Invoke-OllamaReview.ps1
git commit -m "chore: set local Ollama model for AI review"
git push
```

## 5. Test it end to end

1. Open (or pick an existing) pull request on GitHub, note its number.
2. Trigger the workflow manually:
   ```powershell
   gh workflow run ai-code-review.yml -f pr_number=<PR-NUMBER>
   ```
3. Watch it run: repo → **Actions** tab → "AI Code Review (Ollama)" → should
   pick up the job on this runner within a few seconds.
4. Check the PR — a comment from the workflow should appear with
   `VERDICT: APPROVE` or `VERDICT: REQUEST_CHANGES` plus bullet points.

If the job stays queued forever: the runner service isn't running or isn't
registered to this repo — check `Get-Service actions.runner.*` and the
Runners page in Settings.

If `Invoke-RestMethod` fails to reach `localhost:11434`: `ollama serve` isn't
running, or a different port is configured.

## 6. Later: fully automatic

Once step 5 works reliably, switch the trigger in
`.github/workflows/ai-code-review.yml` from manual-only to automatic by
uncommenting the `pull_request` block at the top of the file (instructions
are in a comment right there) and removing the need to pass `pr_number`
manually — that part of the workflow will need a small follow-up edit to read
`github.event.pull_request.number` instead of the manual input. Flag this
back to the other Claude instance / the user before making that change, since
it changes the workflow from opt-in to running on every PR automatically.
