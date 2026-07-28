# HelloSnake

A small C++ / CMake demo project (Snake).

## Build (Windows PowerShell)

```powershell
# Inside the project folder
.\build.ps1

```

### Alternatively (manually with CMake)

```powershell
mkdir build
cd build
cmake ..
cmake --build . --config Debug

```

---

## Repository Setup (one-time)

```bash
echo "# HelloSnake" >> README.md
git init
git add README.md .gitignore LICENSE
git commit -m "Initial commit"
git branch -M main
git remote add origin git@github.com:dasPanjo/HelloSnake.git
git push -u origin main

```
