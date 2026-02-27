# HelloSnake

Kleines C++ / CMake Demo-Projekt (Snake).

Build (Windows PowerShell):

```powershell
# im Projekt-Ordner
.\build.ps1
```

Alternativ (manuell mit CMake):

```powershell
mkdir build
cd build
cmake ..
cmake --build . --config Debug
```

Repository-Setup (einmalig):

```bash
echo "# HelloSnake" >> README.md
git init
git add README.md .gitignore LICENSE
git commit -m "Initial commit"
git branch -M main
git remote add origin git@github.com:dasPanjo/HelloSnake.git
git push -u origin main
```
