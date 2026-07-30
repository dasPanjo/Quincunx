# Coding Guidelines

These guidelines follow common, widely-recognized C++ practice (in the spirit of
the [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines)
and conventions used by most major open-source C++ projects), rather than an
in-house style invented for this repo. Applies to both `engine/Penjin` and `game`.

---

## 1. Naming

| Entity                  | Convention          | Example                          |
|--------------------------|----------------------|-----------------------------------|
| Namespace                | `PascalCase`         | `namespace Penjin`                |
| Class / struct / enum    | `PascalCase`         | `class Window`, `enum class KeyCode` |
| Function / method        | `camelCase`          | `pollEvents()`, `shouldClose()`   |
| Member variable          | `camelCase_` (trailing underscore) | `window_`, `shouldClose_` |
| Local variable / parameter | `camelCase`        | `width`, `windowTitle`             |
| Constant / `constexpr`   | `kPascalCase`        | `kDefaultWidth`                   |
| Macro (avoid where possible) | `ALL_CAPS_SNAKE`  | `PENJIN_ASSERT(x)`                 |
| File name                | matches primary class | `Window.h` / `Window.cpp`        |

Rationale: `camelCase` functions/methods and a trailing underscore on members is
the convention used by Google, LLVM/Clang, Chromium, Abseil and most large C++
codebases — it visually separates members from locals at the call site without
needing an `m_` prefix, and reads close to the standard library's own casing
for functions (`push_back` vs. `pushBack` is a style choice; either is
"standard" — pick one and hold it, this repo uses `camelCase`).

This replaces the earlier ad-hoc suggestion (PascalCase methods, `m_` members)
from when `Application::Run()` was first written — that method should be
renamed to `run()` for consistency going forward.

---

## 2. Files & Headers

- One class per header/source pair, named after the class (`Window.h` / `Window.cpp`).
- `#pragma once` at the top of every header (already used) — simpler than include guards, supported by every compiler this project targets.
- Include order, blank line between groups:
  1. Matching header (in a `.cpp`, include its own `.h` first)
  2. Project headers (`"Application.h"`)
  3. Third-party library headers (`<SDL2/SDL.h>`, `<glad/glad.h>`, `<glm/glm.hpp>`)
  4. Standard library headers (`<iostream>`, `<memory>`)
- Prefer forward declarations in headers over `#include` when only a pointer/reference is needed, to keep compile times down as the codebase grows.
- No `using namespace std;` (or any `using namespace ...;`) in headers. Acceptable sparingly inside a `.cpp` function body, never at file scope.

---

## 3. Language Practices

- **Ownership**: prefer `std::unique_ptr` / `std::shared_ptr` over raw `new`/`delete`. Raw pointers are fine as non-owning references/observers.
- **RAII**: wrap every OS/library resource (SDL window, GL context, file handle) in a class that acquires in the constructor and releases in the destructor — no manual cleanup blocks like the current `Application::run()`.
- Mark overriding virtual methods `override`; mark classes not meant for inheritance `final`.
- Mark methods `const` whenever they don't modify object state.
- Use `nullptr`, never `NULL` or `0` for pointers.
- Use `auto` when the type is obvious from the right-hand side or is a long iterator/template type; spell out the type when it aids readability (e.g. `int`, `float`, small well-known types).
- Pass small/trivial types by value, everything else by `const&` unless the function needs to mutate or take ownership.
- Avoid macros for anything expressible as a `constexpr` value or function.

---

## 4. Formatting

- 4-space indentation, no tabs.
- Opening brace on the same line (`void foo() {`), matching the style already in `Application.cpp`.
- One statement per line.
- Add a `.clang-format` file at the repo root so formatting is enforced by tooling rather than convention alone (recommended next step — ask if you want this added).

---

## 5. Comments

- No comments explaining *what* code does — names should carry that.
- Comment only the *why*: a non-obvious constraint, a workaround, a subtle invariant.
- No commented-out code committed to the repo.

---

## 6. Error Handling

- Use return values / `bool` / `std::optional` for expected failure cases (e.g. "window failed to open").
- Reserve exceptions for truly exceptional, unrecoverable situations.
- Fail loudly during development (assert on programmer errors), fail gracefully on runtime/environment errors (missing files, GPU/driver issues).
