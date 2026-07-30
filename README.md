# Quincunx

A small C++ / CMake demo project.

## Dependencies

Managed via [vcpkg](https://vcpkg.io) in manifest mode (see `vcpkg.json`) — no need to install these manually, vcpkg builds them on first CMake configure:

- [SDL2](https://www.libsdl.org/) — windowing, input, OpenGL context
- [glad](https://github.com/Dav1dde/glad) (feature `gl-api-33`) — OpenGL 3.3 function loader
- [glm](https://github.com/g-truc/glm) — math library (vectors, matrices)

Also required on the system:

- CMake >= 3.15
- A C++17 compiler
- vcpkg, with `VCPKG_ROOT` pointing to your local clone of the [vcpkg repo](https://github.com/microsoft/vcpkg)

Configure with the vcpkg toolchain file so `find_package()` picks up the manifest dependencies:

```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
```

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
echo "# Quincunx" >> README.md
git init
git add README.md .gitignore LICENSE
git commit -m "Initial commit"
git branch -M main
git remote add origin git@github.com:dasPanjo/Quincunx.git
git push -u origin main

```
