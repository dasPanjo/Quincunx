# Quincunx

## Project Design Document (PDD)

Version 0.1

---

# 1. Project Goal

## Main Goal

Quincunx is **not a commercial game**, but primarily a learning project.

The actual goal is to develop a modern, clean, and reusable C++ engine during development, without falling into the typical "engine development for its own sake" trap.

The game serves as a test environment for nearly all important engine systems.

During development, the following topics should be learned hands-on, among others:

* Modern C++
* Software architecture
* Rendering
* OpenGL
* Vulkan (later)
* Game architecture
* Entity management
* Asset pipeline
* Shaders
* CMake
* vcpkg
* Cross-platform development
* Git workflow
* Performance
* Data-oriented design

The game always determines the scope of the engine.

**The engine does not determine the scope of the game.**

---

# 2. Design Philosophy

The engine grows exclusively out of the game's requirements.

This means:

If the game doesn't need audio, no audio system is developed.

If the game doesn't need animations, no animation system is built.

If the game doesn't need physics, no physics is implemented.

This results in a small, clean engine without unnecessary bloat.

---

# 3. Project Vision

The player controls a cube through an abandoned ruin.

The world consists entirely of a fixed grid.

The special gameplay twist:

The number of steps depends on the number on the cube's side.

This creates a puzzle that arises solely from the movement of the cube.

---

# 4. Gameplay

## Player Character

The player is a single cube.

The cube has a number on each side.

For example:

```
Left  = 2

Right = 5

Front = 1

Back  = 4

Top   = 6

Bottom = 3
```

The top side plays **no role**.

Only the side facing the direction of movement matters.

---

## Movement

The player decides:

* Forward
* Backward
* Left
* Right

The game then looks at the corresponding side.

Example:

```
Left side = 3

↓

Key A

↓

The cube moves exactly
3 tiles to the left.
```

Afterwards, the cube rotates.

This changes all the sides.

This in turn changes the number of possible next steps.

This is exactly where the puzzle comes from.

---

# 5. Goal of the Game

The player must find a path through the level.

Not every direction is useful at every moment.

The rotation of the cube affects all future movements.

---

# 6. Setting

The game takes place in an old ruin.

The style should be rather minimalistic.

Envisioned:

* Low poly
* Stone
* Runes
* Warm lighting
* Calm atmosphere

The focus is explicitly not on photorealistic graphics.

---

# 7. MVP (Minimum Viable Product)

## Grid

* Variable
* Start: 10x10
* Later, arbitrary size

---

## Camera

Orbit camera

Features:

* Free mouse rotation
* Zoom
* 90° snap rotation via Q/E

Why?

Because the player must always be able to recognize which number is on which side.

---

## Cube

The cube has:

* Position
* Rotation
* Six side values

It is the central gameplay element.

---

## Level Objects

### Floor

Normal tile.

---

### Wall

Blocks movement.

If the player reaches a wall sooner than their step count would allow, the cube stops directly in front of the wall.

This creates interesting opportunities to deliberately "intercept" movements.

---

### Hole

The hole does not block.

The cube drives into it.

↓

Game lost.

↓

Level restart.

This creates a clear distinction between wall and hole.

---

### Start

Spawn point.

---

### Goal

If the cube reaches the goal:

Level complete.

---

# 8. Why This Gameplay?

The original idea was a Snake variant.

However, over the course of consideration, it became clear that a classic Snake is hard to make believable in a ruin setting.

Additionally, the "snake body" would need to be given a meaningful purpose.

The new idea deliberately reduces the gameplay to a single cube.

This creates a clearer and more original game concept.

---

# 9. Level Design

Initially:

All levels are created by hand.

Why?

Because first it must be understood:

* What is fun?
* How long should a level be?
* Which obstacles work?
* Which combinations create interesting decisions?

Only afterwards is procedural generation worthwhile.

---

# 10. Later Extensions

Once the first levels are fun:

Then:

* Multiple levels
* Menu
* Level selection
* Saving
* Animations
* Sound
* Music

After that:

Endless mode.

Here a generator can automatically create levels.

---

# 11. Technical Stack

## Operating System

Linux
Windows

---

## Language

Modern C++

---

## Build System

CMake

---

## Package Manager

vcpkg

---

## Windowing

SDL3

---

## Rendering

OpenGL

loaded via

GLAD

---

## Future

OpenGL

↓

Vulkan

without changes to the game logic.

---

# 12. Architecture

The most important architectural goal is:

The game logic never knows about OpenGL.

The layers look like this:

```
Application
        │
        ▼
Game
        │
        ▼
Renderer Interface
        │
        ▼
OpenGL Renderer
```

Later:

```
Application
        │
        ▼
Game
        │
        ▼
Renderer Interface
        │
        ▼
Vulkan Renderer
```

Only the backend is swapped.

The game remains identical.

---

## Engine Services (Logger, Time, Input)

Cross-cutting engine services that need to be reachable from arbitrary,
deeply-nested code — not just from `Application` or `Game` directly — are
implemented as singletons with a `static X& get()` accessor: `Logger`,
`Time`, and `Input`.

This matters once the planned Component/Behaviour system (see Roadmap,
Phase 7) exists: a `Behaviour` attached to a `GameObject` is called back by
the engine (`onUpdate()` or similar) without control over the parameter
list, and can sit arbitrarily deep in a `Scene → GameObject → Component`
hierarchy. Threading `Input&`/`Time&` through every layer of that hierarchy
just so a leaf `Behaviour` can ask "is W pressed?" or "what's delta time?"
would mean touching every intermediate class whenever a new service is
added — the same reasoning that already justified `Logger` being callable
from anywhere via `LOG_ERROR(...)` without a passed-in reference.

Concretely: `Time::get().deltaTime()` and `Input::get().isKeyDown(KeyCode::W)`
are callable from any `Behaviour`, regardless of nesting, without the engine
needing to pass a context object down through every intermediate owner.

The trade-off is accepted deliberately: this makes having two independent
input/time states (e.g. local split-screen, or isolated unit tests) harder
to retrofit later. That's judged acceptable for now — Quincunx is single
window, single player — and consistent with the project's "no premature
generalization" principle (Development Principles, #5).

---

# 13. Git Strategy

Engine and game are developed together, in one repository.

The reasoning: while Penjin is still young, most work happens in lockstep across both — a game requirement drives an engine change, which is immediately tested in the game. Splitting into two repos with a submodule from day one would mean building the engine, pushing it, then bumping the submodule pointer in the game just to test a one-line change. That overhead isn't worth it yet.

## Quincunx (this repo)

One repository, one CMake build, one `cmake --build` for both:

* `engine/Penjin` — rendering, input, window, assets, math, renderer, core. No game logic.
* `game` — gameplay, level, UI, states. No rendering backend details.

The two stay code-separated (game never includes OpenGL/SDL directly, only Penjin's interfaces) even though they live in one repo — that boundary is what makes the later split possible.

---

## Splitting Penjin out later

Once Penjin is stable enough to be useful as a standalone library (e.g. for a second game), it gets extracted into its own repository — with full history — using:

```bash
git subtree split -P engine/Penjin -b penjin-standalone
```

The resulting branch contains only the commits touching `engine/Penjin`, rewritten as if it always lived at the repo root. Push that branch to a new, empty repository and Penjin becomes a standalone library with its own history.

From that point on, this repository would consume Penjin the way the original plan described — as a Git submodule (or vendored copy) — since active co-development is no longer required.

---

# 14. Project Structure

```
Quincunx/
│
├── engine/
│   └── Penjin/
│       ├── CMakeLists.txt
│       └── src/
│           └── core/
│
├── game/
│   ├── CMakeLists.txt
│   └── src/
│       └── Main.cpp
│
├── assets/
│   ├── models/
│   ├── textures/
│   ├── shaders/
│   └── fonts/
│
├── levels/
│
├── docs/
│
├── CMakeLists.txt
└── vcpkg.json
```

The root `CMakeLists.txt` owns the single top-level project and pulls in both `engine/Penjin` and `game` via `add_subdirectory` — no relative-path reaching between them, no separate configure/build step per component.

---

# 15. Roadmap

## Phase 1 – Project Setup

* Create Git repositories
* Configure CMake
* Set up vcpkg
* Integrate SDL3
* Integrate GLAD
* Create OpenGL context
* Display first window

**Result:** A running window with a main loop.

---

## Phase 2 – Engine Foundations

* Logger
* Time system
* Input system
* Window wrapper
* Camera class
* Renderer interface (`IRenderer`)
* OpenGLRenderer

**Result:** The engine can render an empty grid.

---

## Phase 3 – Rendering

* Render cube
* Render grid
* Shader system
* Camera
* Orbit control
* 90° snap rotation
* Lighting

**Result:** A freely viewable 3D scene.

---

## Phase 4 – Gameplay

* Cube rotation
* Side values
* Movement logic
* Grid collision
* Movement based on side values

**Result:** The core of the game is playable.

---

## Phase 5 – Level

* Walls
* Holes
* Start
* Goal
* Restart
* Win condition

**Result:** The first fully playable level.

---

## Phase 6 – Content

* Additional levels
* Menu
* UI
* Loading screen
* First ruin assets
* Sound effects

---

## Phase 7 – Engine Expansion

Only now are new systems added, when the game needs them:

* Asset manager
* Audio
* Particles
* Animations
* Editor tools

---

## Phase 8 – Procedural Levels

Once enough handcrafted levels exist and it's clear what makes a good level:

* Generator
* Validation
* Endless mode

---

## Phase 9 – Vulkan

The last big step:

* Implement Vulkan backend
* Make OpenGL backend swappable
* Leave game logic unchanged

---

# 16. Development Principles

A few simple rules apply throughout development:

1. **The game always takes priority over the engine.** New engine systems are created only when the game truly needs them.
2. **A playable version should exist every week.** Even if it consists of just a cube on a grid.
3. **Small, completed milestones.** Ten small successes are better than one giant, unfinished goal.
4. **Function first, then optimization.** Readable, clean code is more important than premature micro-optimizations.
5. **No premature generalization.** Only abstract when at least two concrete use cases exist.
6. **Game logic and rendering remain strictly separated.** This later enables swapping the graphics backend from OpenGL to Vulkan.
