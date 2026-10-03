# Phase 3 — Rendering: Detailed Checklist

Expands the five one-line bullets in `Project Design Document.md`'s
"Phase 3 – Rendering" section into an ordered, actionable checklist, plus the
concrete design for the Unity-like scene system (`GameObject`/`Component`/
`Transform`) that the rest of Phase 3 — and every later phase — builds on.

Sections marked **[deep-designed]** below include full class designs, ready
to implement close to as-written. Sections marked **[checklist-only]** are
deliberately not fully designed yet — they get their own dedicated design
pass later, the same way `Input`, `Logger`, `Time`, and the `IRenderer`/
`OpenGLRenderer` split were each designed in one focused pass before being
built.

---

## 1. Scene System Design (GameObject / Component / Transform)

### Why Transform is its own class, not folded into GameObject

`GameObject` owns exactly one `Transform`, as a non-optional, non-removable,
by-value member (`Transform transform_`) — **not** an entry in the generic
component list. `Transform` itself owns the parent/children pointers,
matching the `transform.parent` mental model directly.

Reasoning:
- Different axis of change: spatial math/hierarchy vs. identity and
  component bookkeeping are two unrelated reasons to change a class.
- Protects the "every component is optional/removable" invariant of the
  generic component list. If `Transform` were just another component,
  `getComponent<Transform>()` could return null, and every consumer
  (including engine code like `MeshRenderer`) would need to defensively
  null-check spatial data that should always exist.

### `Component` base class

`engine/Penjin/src/scene/Component.h` / `.cpp`:

```cpp
class Component {
    friend class GameObject;
public:
    virtual ~Component() = default;

    GameObject& gameObject() const { return *gameObject_; }
    Transform& transform() const;          // defined in .cpp, forwards to gameObject_->transform()

    virtual void onStart() {}
    virtual void onUpdate() {}

protected:
    Component() = default;

private:
    void attachTo(GameObject* owner) { gameObject_ = owner; }
    GameObject* gameObject_ = nullptr;
};
```

Only two lifecycle hooks, each with a concrete near-term caller:
- `onUpdate()` — driven every frame by `GameObject::update()`.
- `onStart()` — called synchronously, once, immediately inside
  `addComponent<T>()`. Not deferred like Unity's `Awake`/`Start` split,
  since nothing here has a cross-object init-order dependency yet.

Deliberately **not** included:
- `onAttach()` — redundant with eager `onStart()`.
- `onDestroy()` — C++ destructors already give deterministic cleanup; a
  derived class overrides its own destructor instead.
- A `deltaTime` parameter on `onUpdate()` — components call
  `Time::get().deltaTime()` / `Input::get()...` directly, consistent with
  the PDD's "Engine Services" section, which already justifies `Time`/
  `Input`/`Logger` being singletons specifically so this kind of leaf-level
  access doesn't need to be threaded through `Scene → GameObject →
  Component`.

Named `Component`, not `IComponent` — the `I`-prefix in this codebase
(`IRenderer`, `ILogSink`) is reserved for pure-virtual interfaces;
`Component` has real default bodies, making it a base class, not an
interface.

### `GameObject`

`engine/Penjin/src/scene/GameObject.h` / `.cpp`:

```cpp
class GameObject final {
public:
    explicit GameObject(std::string name = "GameObject");
    ~GameObject();

    const std::string& name() const { return name_; }
    Transform& transform() { return transform_; }
    const Transform& transform() const { return transform_; }

    template <typename T, typename... Args>
    T& addComponent(Args&&... args) {
        static_assert(std::is_base_of_v<Component, T>, "T must derive from Penjin::Component");
        auto owned = std::make_unique<T>(std::forward<Args>(args)...);
        T& ref = *owned;
        ref.attachTo(this);
        components_.push_back(std::move(owned));
        ref.onStart();
        return ref;
    }

    template <typename T> T* getComponent() const;              // dynamic_cast scan, nullptr if absent
    template <typename T> std::vector<T*> getComponents() const;

    void update();                        // calls onUpdate() on every component
    void draw(IRenderer& renderer) const; // special-cased MeshRenderer lookup, see §3

private:
    std::string name_;
    Transform transform_;                                   // destroyed after components_ (see below)
    std::vector<std::unique_ptr<Component>> components_;
};
```

**This is the mechanism that satisfies "components written outside the
engine, zero registration":** `addComponent`/`getComponent`/`getComponents`
are templates, instantiated fresh wherever they're called. The engine
library never enumerates or gets recompiled for any specific `T` — a
`class PlayerController : public Penjin::Component` defined entirely in
`game/src/` works with zero engine-side changes, since `Component` is just a
public base class any `game/` file can `#include` and derive from.

- `getComponent<T>`/`getComponents<T>` use `dynamic_cast` over `components_`
  (requires `Component` to be polymorphic — satisfied by the virtual
  destructor). Chosen over a manual type-tag/registry because it needs zero
  registration and naturally supports querying by an intermediate base type
  later (e.g. a future `getComponent<Collider>()` matching either
  `BoxCollider` or `SphereCollider`).
- Ownership: `std::vector<std::unique_ptr<Component>>` — `GameObject` is the
  sole owner; component lifetime equals GameObject lifetime.
- Deliberately **not added yet** (no concrete near-term caller):
  `removeComponent`, an active/enabled flag on GameObject. Both are cheap,
  purely additive extensions later.
- **Member order matters**: `name_`, `transform_`, `components_` in that
  declaration order — C++ destroys members in *reverse* order, so
  `components_` (and any component destructor that calls
  `transform()`/`gameObject()`) runs before `transform_` is gone.
- `~GameObject()` must be declared in the header and defined (`= default;`)
  in `GameObject.cpp`, not inline in the header — that's where
  `Component`'s complete type is visible, needed to destroy
  `unique_ptr<Component>` entries correctly.

### Parent/child hierarchy (lives on `Transform`)

`engine/Penjin/src/scene/Transform.h` / `.cpp`:

```cpp
class Transform final {
public:
    explicit Transform(GameObject* gameObject);
    ~Transform();  // unlinks from parent, orphans children

    GameObject& gameObject() const { return *gameObject_; }

    glm::vec3 localPosition{0.0f};
    glm::quat localRotation{1.0f, 0.0f, 0.0f, 0.0f};
    glm::vec3 localScale{1.0f};

    glm::mat4 localMatrix() const;
    glm::mat4 worldMatrix() const;
    glm::vec3 worldPosition() const;
    glm::quat worldRotation() const;
    glm::vec3 worldScale() const;      // "lossy" scale, same caveat as Unity's transform.lossyScale

    Transform* parent() const { return parent_; }
    const std::vector<Transform*>& children() const { return children_; }
    void setParent(Transform* parent, bool worldPositionStays = true);

private:
    GameObject* gameObject_;
    Transform* parent_ = nullptr;
    std::vector<Transform*> children_;

    void detachFromParent();
};
```

- Parent/children are **non-owning** `Transform*` — ownership of
  `GameObject`s stays entirely in `Scene` (flat list, see §1.4). Pointer
  addresses stay stable because `Scene` stores `std::unique_ptr<GameObject>`
  (never `std::vector<GameObject>`, which would invalidate addresses on
  reallocation).
- World matrix composition, walking the parent chain, recomputed fresh on
  every call (no dirty-flag caching — revisit only if profiling ever shows
  it matters at this project's scale):
  ```
  worldMatrix() = parent_ ? parent_->worldMatrix() * localMatrix() : localMatrix()
  localMatrix() = translate(localPosition) * mat4_cast(localRotation) * scale(localScale)
  ```
- `setParent(newParent, worldPositionStays = true)` **preserves world
  position/rotation/scale by default** — matches Unity's own default.
  Algorithm: capture current world position/rotation/scale before
  relinking, do the pointer surgery, then re-solve local values that
  reproduce the captured world values under the new parent. If
  `worldPositionStays == false`, skip the re-solve (object visually jumps),
  matching Unity's `SetParent(parent, false)` overload.
  - This is a deliberate exception to "no premature generalization": the
    implementation is small and self-contained, it's a foundational
    semantic that's cheap to get right now and expensive to change the
    default of later, and it costs nothing for the near-term grid-tile
    hierarchy use (§3), where the math is a no-op either way.
- `~Transform()`: removes `this` from `parent_->children_` if a parent
  exists; orphans (not destroys) any remaining children by setting their
  `parent_ = nullptr`. No cascading destruction — nothing in Phase 3/4
  destroys GameObjects at runtime, so this isn't needed yet.
- **Known sharp edge, not solved now**: `setParent` guards against
  `newParent == this` but not deeper cycles (parenting to a grandchild). No
  near-term caller does dynamic multi-level reparenting.

### `Scene`

`engine/Penjin/src/scene/Scene.h` / `.cpp` — minimal, flat, deliberately not
an ECS:

```cpp
class Scene final {
public:
    GameObject& createGameObject(std::string name = "GameObject");
    void update();                        // gameObject->update() for each
    void draw(IRenderer& renderer) const;  // gameObject->draw(renderer) for each
    const std::vector<std::unique_ptr<GameObject>>& gameObjects() const { return gameObjects_; }

private:
    std::vector<std::unique_ptr<GameObject>> gameObjects_;
};
```

Owns *all* GameObjects flatly (root and children alike) — the parent/child
tree is a `Transform`-level relationship layered on top, orthogonal to
ownership and to update/draw order. This avoids the real complexity of
transferring `unique_ptr<GameObject>` ownership between tree nodes on
reparent, and matches how Unity itself actually owns objects internally
(the Hierarchy panel's nesting is a `Transform.parent` display, not the real
ownership model). No ECS, query system, or scene serialization — not needed
to make GameObject/Component/Transform/MeshRenderer usable end-to-end.

**Known sharp edge, not solved now**: mutating `components_`/`gameObjects_`
from inside an `onUpdate()` call invalidates the iteration in progress — no
guard exists yet. Nothing planned needs this; the standard fix later is a
deferred "pending additions" queue flushed after the update loop.

### Wiring into the app loop

- [ ] New `game/src/QuincunxGame.h` / `.cpp` — `class QuincunxGame final :
      public Penjin::Application`, owns a `Scene scene_;`, overrides
      `tick()` to call `scene_.update()` and `draw()` to call
      `scene_.draw(*renderer_)` (using `Application`'s existing protected
      `renderer_`).
- [ ] `game/src/Main.cpp` — construct `QuincunxGame` instead of
      `Penjin::Application` directly.
- [ ] Throwaway `game/src/PlayerController.h` / `.cpp` — derives
      `Penjin::Component`, overrides `onUpdate()` calling
      `Input::get()`/`Time::get()`, attached via
      `gameObject.addComponent<PlayerController>()`. Concrete proof that
      the zero-registration extensibility requirement holds end-to-end; can
      be deleted once real Phase 4 gameplay components exist.

No `Quincunx` namespace for `QuincunxGame`/example components — with only a
symbol or two, that's namespace ceremony with no near-term collision to
prevent, consistent with `Main.cpp` already sitting at global scope today.

### Folder/namespace placement

Two new folders under `engine/Penjin/src/`, both **flat under `Penjin`** —
no nested namespace:

- **`scene/`** — `Component`, `Transform`, `GameObject`, `Scene`,
  `MeshRenderer`.
- **`graphics/`** — `Vertex`, `Mesh`, `Material`, `Shader`.

**Why flat**: 7 of the 8 existing subsystems (`Window`, `Input`, `Time`,
`KeyCode`/`MouseButton`, `IRenderer`/`OpenGLRenderer`) are already flat under
`Penjin`. Only `logger/` nested (`Penjin::Logger`), and that nesting is
exactly what produced the awkward `Penjin::Logger::Logger` repeat — nesting
`scene/` would reproduce the identical `Penjin::Scene::Scene` collision, a
concrete, specific reason to avoid it here.

Dependency direction stays acyclic:

```
graphics (Vertex, Mesh, Material, Shader)   — depends on nothing else in Penjin (just glad + glm)
   ^
renderer (IRenderer, OpenGLRenderer)        — depends on graphics (drawMesh's Mesh/Material params)
   ^
scene (Component, Transform, GameObject,    — depends on renderer (IRenderer&) and graphics
       Scene, MeshRenderer)                    (MeshRenderer's Mesh/Material members)
```

**Implementation-time reminder**: `engine/Penjin/src/CMakeLists.txt`'s
`target_include_directories(Penjin PUBLIC ...)` currently only lists `core`
and `window`. Add `scene`, `graphics`, and `renderer` to that list once these
folders exist, or `game/src`'s `#include <GameObject.h>`-style includes
won't resolve.

---

## 2. Shader System (minimal) — [deep-designed, minimal version]

- [ ] `Shader` class — `engine/Penjin/src/graphics/Shader.h` / `.cpp`:
  ```cpp
  class Shader final {
  public:
      static std::shared_ptr<Shader> createFromSource(const std::string& vertexSource, const std::string& fragmentSource);
      static std::shared_ptr<Shader> createUnlitColor();  // built-in, inline GLSL
      ~Shader();  // glDeleteProgram
      void bind() const;
      void setMat4(const std::string& name, const glm::mat4& value) const;
      void setVec4(const std::string& name, const glm::vec4& value) const;
  private:
      unsigned int programId_ = 0;
  };
  ```
- [x] `Shader::createUnlitColor()` — GLSL source as **inline string
      literals** in `Shader.cpp`, not loaded from asset files. There is
      currently zero asset-loading/path-resolution infrastructure in the
      project (`assets/` doesn't exist on disk yet); building one just to
      load two small strings is unjustified now — real asset loading is
      Phase 7 scope. `#version 330 core`, matching the existing GL context
      (`SDL_GL_CONTEXT_MAJOR/MINOR_VERSION = 3.3`) and ImGui's own
      `ImGui_ImplOpenGL3_Init("#version 330")` already in `Window.cpp`.
      ```glsl
      // vertex
      layout(location = 0) in vec3 aPosition;
      layout(location = 1) in vec3 aNormal;
      layout(location = 2) in vec2 aUV;
      uniform mat4 uModel; uniform mat4 uView; uniform mat4 uProjection;
      void main() { gl_Position = uProjection * uView * uModel * vec4(aPosition, 1.0); }

      // fragment
      out vec4 FragColor;
      uniform vec4 uColor;
      void main() { FragColor = uColor; }
      ```
- [x] Extend `IRenderer` (`engine/Penjin/src/renderer/IRenderer.h`, existing
      file) with two new pure-virtual methods, plus forward declarations of
      `Mesh`/`Material` and `#include <glm/mat4x4.hpp>`:
  ```cpp
  virtual void setViewProjection(const glm::mat4& view, const glm::mat4& projection) = 0;
  virtual void drawMesh(const Mesh& mesh, const Material& material, const glm::mat4& modelMatrix) = 0;
  ```
- [ ] Implement both in `OpenGLRenderer` — store `currentView_`/
      `currentProjection_` as private state (set once per frame via
      `setViewProjection`, analogous to the existing `beginFrame(clearColor)`
      pattern); `drawMesh` binds material → shader, sets `uModel`/`uView`/
      `uProjection`, binds mesh, `glDrawElements`:
  ```cpp
  void OpenGLRenderer::drawMesh(const Mesh& mesh, const Material& material, const glm::mat4& modelMatrix) {
      material.bind();
      material.shader().setMat4("uModel", modelMatrix);
      material.shader().setMat4("uView", currentView_);
      material.shader().setMat4("uProjection", currentProjection_);
      mesh.bind();
      glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh.indexCount()), GL_UNSIGNED_INT, nullptr);
  }
  ```

**Why the split (per-frame state vs. per-object draw call)**: this
deliberately mirrors the same reasoning as `Input`/`Time` being singletons.
If view/projection were passed as explicit parameters through
`Scene::draw → GameObject::draw → MeshRenderer::draw → drawMesh`, every one
of those signatures would need to grow again the moment Camera (§4) — and
later Lighting — is added. Because Camera doesn't exist yet, "Render Cube"
(§3) temporarily calls `setViewProjection` with a hardcoded
`glm::lookAt`/`glm::perspective` pair, to be replaced later at that exact
call site with zero signature changes anywhere else.

- [ ] **[checklist-only, future]** Load shader source from
      `assets/shaders/*.vert`/`*.frag` once an asset-loading convention
      exists (Phase 7) — explicitly not now.
- [ ] **[checklist-only, future]** Uniform location caching (currently
      `glGetUniformLocation` by name every call) — revisit only if
      profiling shows it matters.

---

## 3. Render Cube / Render Grid — [deep-designed]

- [ ] `Vertex` struct — `engine/Penjin/src/graphics/Vertex.h` (header-only):
  ```cpp
  struct Vertex { glm::vec3 position; glm::vec3 normal; glm::vec2 uv; };
  ```
  All three fields now, even though hand-built primitives barely need
  `normal`/`uv` yet — this is the concrete mechanism behind "prepared for
  Assimp": a future loader converts `aiMesh` data into
  `std::vector<Vertex>` with **zero struct changes**, whereas adding a
  field later would force revisiting the VAO attribute layout
  (`glVertexAttribPointer` offsets, coupled to `sizeof(Vertex)`) and the
  vertex shader's `layout(location=N)` assumptions. Computing correct
  per-face normals/UVs for hand-built cube/plane geometry is cheap and
  well-understood — unlike genuinely speculative data (e.g. bone weights),
  which is *not* added now.

- [ ] `Mesh` class — `engine/Penjin/src/graphics/Mesh.h` / `.cpp` — **plain
      OpenGL-concrete on purpose**:
  ```cpp
  class Mesh final {
  public:
      Mesh(const std::vector<Vertex>& vertices, const std::vector<std::uint32_t>& indices);
      ~Mesh();  // glDeleteVertexArrays/Buffers

      static std::shared_ptr<Mesh> createCube(float size = 1.0f);
      static std::shared_ptr<Mesh> createPlane(float width = 1.0f, float depth = 1.0f);

      void bind() const;   // glBindVertexArray -- keeps the VAO id private
      std::uint32_t indexCount() const { return indexCount_; }

  private:
      unsigned int vao_ = 0, vbo_ = 0, ebo_ = 0;
      std::uint32_t indexCount_ = 0;
  };
  ```
  - No `IMesh` backend-agnostic abstraction — deliberately deferred until
    Vulkan is a reality in the far-future Phase 9, not an oversight. This
    doesn't violate "game logic never touches OpenGL": that rule is about
    `game/src`, and `Mesh` is engine code — `game/src` only ever calls
    `Mesh::createCube()`, same boundary `Window`'s direct SDL/GL calls
    already establish as acceptable for engine-internal code.
  - `Mesh.h` uses plain `unsigned int`/`std::uint32_t` for handles, never
    includes `<glad/glad.h>` — only `Mesh.cpp` does, keeping glad out of
    every transitive consumer (`MeshRenderer.h` → `Material.h`/`Mesh.h`).
  - `createCube()` builds **24 vertices** (4 per face × 6, not 8 shared
    corners) and 36 indices — a shared-corner cube can't have correct
    per-face flat normals. Standard, well-known low-poly-cube
    construction; sets up "Lighting" (§4) to work without revisiting this
    factory.
  - `createPlane()`: 4 vertices, 6 indices, in the XZ plane, normal
    `(0,1,0)`, centered at local origin — the grid tile / floor shape.
  - Both `Mesh` and `Material` are held via `std::shared_ptr` (not by
    value, not `unique_ptr`) specifically so **grid tiles share one
    `Mesh::createPlane()` instance** across all ~100 tiles (a concrete,
    near-term win — the grid is explicitly 10×10 now per the PDD's MVP)
    rather than allocating N redundant VAOs for identical quads.
  - No caching/dedup system built now — avoiding redundant
    `createCube()`/`createPlane()` calls is the *caller's* responsibility
    (reuse the `shared_ptr` you already have); a real asset cache/manager
    is Phase 7 scope.

- [ ] `Material` class — `engine/Penjin/src/graphics/Material.h` / `.cpp`:
  ```cpp
  class Material final {
  public:
      explicit Material(std::shared_ptr<Shader> shader, glm::vec4 color = glm::vec4(1.0f));
      Shader& shader() const { return *shader_; }
      glm::vec4 color() const { return color_; }
      void setColor(const glm::vec4& color) { color_ = color; }
      void bind() const;  // shader_->bind(); shader_->setVec4("uColor", color_);
  private:
      std::shared_ptr<Shader> shader_;
      glm::vec4 color_;
      // future: textures / additional uniforms -- not added now, this is
      // the seam Assimp-loaded materials slot into later
  };
  ```
  `shared_ptr<Shader>` because many Materials (the cube's, every grid
  tile's) legitimately share one compiled program, differing only by
  `color_`.

- [ ] `MeshRenderer` component — `../engine/Penjin/src/mesh/MeshRenderer.h` /
      `.cpp`:
  ```cpp
  class MeshRenderer final : public Component {
  public:
      MeshRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<Material> material);
      const std::shared_ptr<Mesh>& mesh() const { return mesh_; }
      void setMesh(std::shared_ptr<Mesh> mesh) { mesh_ = std::move(mesh); }
      const std::shared_ptr<Material>& material() const { return material_; }
      void setMaterial(std::shared_ptr<Material> material) { material_ = std::move(material); }
      void draw(IRenderer& renderer) const {
          if (mesh_ && material_) renderer.drawMesh(*mesh_, *material_, transform().worldMatrix());
      }
  private:
      std::shared_ptr<Mesh> mesh_;
      std::shared_ptr<Material> material_;
  };
  ```
  Not driven via `onUpdate()` — it's passive data. `GameObject::draw()`
  special-cases a `getComponent<MeshRenderer>()` lookup:
  ```cpp
  void GameObject::draw(IRenderer& renderer) const {
      if (const MeshRenderer* meshRenderer = getComponent<MeshRenderer>()) meshRenderer->draw(renderer);
  }
  ```
  Deliberately not a generic `virtual Component::onDraw()` — no second
  drawable component type exists yet to justify that generalization. If one
  shows up later, the natural extension is another `getComponent<T>()`
  check here, or promoting to a small `IDrawable` — correctly left for
  then.

### Render Cube build steps

- [ ] Enable depth testing — `glEnable(GL_DEPTH_TEST)` in
      `OpenGLRenderer::init()`, and add `GL_DEPTH_BUFFER_BIT` to
      `beginFrame()`'s `glClear(...)` call. **Verified currently absent**
      from `OpenGLRenderer.cpp` — a 3D cube renders incorrectly without
      this.
- [ ] Scene-construction glue (in `QuincunxGame`): create a `"Cube"`
      GameObject, `addComponent<MeshRenderer>(Mesh::createCube(),
      std::make_shared<Material>(Shader::createUnlitColor()))`.
- [ ] Temporary hardcoded `renderer_->setViewProjection(...)` call (fixed
      `glm::lookAt`/`glm::perspective`) since Camera doesn't exist yet — to
      be replaced later at this same call site with zero signature changes.
- [ ] Verify: cube visible, correctly proportioned, no draw-order
      artifacts.

### Render Grid build steps

- [ ] Reuse `Mesh::createPlane()` (already built above).
- [ ] Scene-construction glue: one shared `shared_ptr<Mesh>` plane, reused
      across all grid tile GameObjects (10×10 per the PDD MVP) — avoids N
      redundant VAOs.
- [ ] Parent all tile GameObjects under one root `"Grid"` GameObject via
      `transform().setParent(...)` — a real, concrete exercise of the
      hierarchy (not speculative), and lets the whole grid move later by
      moving one parent.
- [ ] Verify: flat tile grid renders beneath/around the cube.

---

## 4. Camera, Orbit Control, 90° Snap Rotation, Lighting — [checklist-only]

Not fully class-designed yet — each gets its own dedicated design pass
later, the same way Input/Logger/Time were each designed before being
built. Listed here with enough detail to be a real checklist, not just a
restatement of the PDD's one-line bullets.

### Camera
- [ ] Decide architectural placement: standalone class owned by
      `Scene`/`QuincunxGame`, vs. a `Component` on a "Main Camera"
      GameObject — deferred to its own design pass.
- [ ] New file(s): likely `engine/Penjin/src/camera/Camera.h` / `.cpp`
      (flat `Penjin` namespace). Responsibility: hold
      position/orientation/fov/near/far/aspect, compute `viewMatrix()`
      (`glm::lookAt`) and `projectionMatrix()` (`glm::perspective`).
- [ ] Wire Camera's output into the existing `IRenderer::
      setViewProjection(...)` seam (§2) — no other signature changes
      needed.
- [ ] Handle window resize — **verified currently unhandled**: no
      `SDL_EVENT_WINDOW_RESIZED` case in `Window::pollEvents()`, no
      `glViewport` call anywhere. Needed so Camera's aspect ratio and the
      GL viewport stay correct after a resize.

### Orbit Control
- [ ] Depends on Camera existing first.
- [ ] New file: e.g. `engine/Penjin/src/camera/OrbitCameraController.h` /
      `.cpp` (or a `Component`, depending on Camera's chosen placement) —
      reads `Input::get().mouseDelta()` (drag → yaw/pitch around a target)
      and `Input::get().scrollDelta()` (zoom/distance), both already
      exposed by the existing `Input` class.
- [ ] Clamp pitch to avoid flipping over the pole; decide the orbit target
      (world origin, grid center, or the cube's position).

### 90° Snap Rotation
- [ ] Depends on Orbit Control's yaw state existing.
- [ ] Reads `Input::get().isKeyPressed(KeyCode::Q)`/`KeyCode::E` (both
      already defined) to snap the camera's yaw by ±90°.
- [ ] Instant snap first; optional smoothing pass afterward using
      `Time::get().deltaTime()` to interpolate — ties directly back to the
      existing `Time` singleton.

### Lighting
- [ ] Simplest viable model: one hardcoded directional light (matches the
      PDD's "warm lighting, calm atmosphere" setting), not a general
      multi-light system — additional generality deferred until a second
      concrete use case exists.
- [ ] Extend the fragment shader from flat-unlit to basic Lambertian
      diffuse — needs `normal`, already present on `Vertex` (§3),
      confirming the forward-compatibility payoff.
- [ ] Extend `IRenderer` with a per-frame `setLighting(...)`-style call,
      reusing the exact ambient-per-frame-state pattern established for
      `setViewProjection` (§2) — not threaded through `Scene`/`GameObject`/
      `MeshRenderer` signatures.
- [ ] Normal-matrix handling (`mat3(transpose(inverse(model)))`) — open
      detail for that dedicated pass.
- [ ] Full lighting design (light count, Component-based vs. hardcoded,
      shadows) explicitly out of scope here.

---

## Result

A freely viewable 3D scene: an orbit-controllable camera looking at a lit
cube standing on a grid, built on a reusable GameObject/Component/Transform
foundation that Phase 4 (gameplay) and later phases (content, procedural
levels) build directly on top of.
