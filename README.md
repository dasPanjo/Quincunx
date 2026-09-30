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

## How to contribute (Linux)

If vcpkg is not installed, set it up with:

```bash
git clone https://github.com/microsoft/vcpkg.git ~/vcpkg
~/vcpkg/bootstrap-vcpkg.sh
export VCPKG_ROOT=~/vcpkg
```

Add the `export VCPKG_ROOT=~/vcpkg` line to `~/.bashrc` (or equivalent) so it persists across shell sessions.

Then configure and build the project:

```bash
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
cmake --build build
```

Requirements:

- CMake >= 3.15
- A C++17 compiler (e.g. `g++` or `clang++`)
- Build tools for vcpkg itself (e.g. `build-essential`, `curl`, `zip`, `unzip`, `tar`, `pkg-config` on Debian/Ubuntu)
- `autoconf`, `automake`, `libtool`, `autoconf-archive` — required by vcpkg to build `libxcrypt` (a transitive dependency of SDL3 on Linux):
  ```bash
  sudo apt install autoconf automake libtool autoconf-archive
  ```

### CLion (Linux)

CLion's built-in vcpkg integration (Settings → Build, Execution, Deployment → vcpkg) is not available on Linux. Instead, set the toolchain file manually:

1. Settings → Build, Execution, Deployment → **CMake**
2. For each profile (e.g. Debug), add to **CMake options**:
   ```
   -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake
   ```
3. **Apply**, then **"Reset Cache and Reload Project"** (a plain reload keeps the stale cache and the error persists).

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
