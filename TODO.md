# TODO — Conduit Engine

**Single source of truth for what happens next.** Take the topmost unblocked task.
Read [`AGENTS.md`](./AGENTS.md) first. Completed items move to [`DONE.md`](./DONE.md)
in the same commit as the work.

**Status:** Pre-alpha. Version `0.1.0-dev`. Phase 0 complete. Phase 1 is underway;
fundamental types are complete and `Handle<T>` is next.

### Conventions
- `[ ]` open · `[~]` in progress · `[!]` blocked (reason must be stated)
- **Dep:** names the task that must land first. Do not skip ahead past an unbuilt dependency.
- Phases are ordered by dependency, not by preference. Finish a phase before starting the next,
  unless a task is explicitly marked parallel-safe.

---

## Phase 0 — Foundation ✅ *complete — see [`DONE.md`](./DONE.md)*

## Phase 1 — Core  ← *start here*

- [ ] **1.2 `Handle<T>`.** Opaque index + 16-bit generation counter. Stale handles must resolve to
      null, never to a recycled object. This is the backbone of every public engine API. **Dep:** 1.1
- [ ] **1.3 Assertions & logging.** `CONDUIT_ASSERT` compiled out in release; a leveled logger with
      pluggable sink so the editor console can attach. No allocation on the logging fast path. **Dep:** 1.1
- [ ] **1.4 Allocators.** Frame arena (bump allocator, reset per tick) and a pool allocator for
      fixed-size objects. These make the "no per-frame allocation" rule enforceable. **Dep:** 1.1
- [ ] **1.5 Math.** `Vec2`, `Mat3`, `Rect`, `Transform2D`, `Color`. Header-only, constexpr where
      possible, tested against known values. **Dep:** 1.1

## Phase 2 — Platform Abstraction

- [ ] **2.1 `IWindow`.** Pure interface: create/destroy, size, DPI scale, event pump, swap.
      Zero platform types in the signatures. **Dep:** 1.3
- [ ] **2.2 `IInput`.** Unified event model spanning keyboard, mouse, and **touch** from day one —
      retrofitting touch later is how engines end up desktop-only. **Dep:** 1.3
- [ ] **2.3 `IRenderer`.** Device/command abstraction: buffers, textures, shaders, draw submission.
      Deliberately narrow — it must be implementable on GL, GLES, Metal, and Vulkan. **Dep:** 1.2
- [ ] **2.4 Desktop backend.** First concrete implementation (SDL3 or native win32/x11/cocoa —
      decide and record an ADR in `docs/adr/`). Opens a window, pumps input, clears to a color. **Dep:** 2.1, 2.2, 2.3
- [ ] **2.5 OpenGL 3.3 / GLES 3.0 device.** First `IRenderer` implementation. **Dep:** 2.3
- [ ] **2.6 Engine loop.** Fixed-timestep update, variable render, frame arena reset, clean shutdown.
      **Dep:** 2.4, 2.5, 1.4

## Phase 3 — Scene & Node Model

- [ ] **3.1 `Node`.** Name, parent, children, `Transform2D`, enabled/visible flags. Contiguous
      storage; the tree is an index structure over an array, not a web of pointers. **Dep:** 1.2, 1.5
- [ ] **3.2 `NodeTree`.** Ownership, add/remove/reparent, stable handles across mutation. **Dep:** 3.1
- [ ] **3.3 Path resolution.** `GetNode("Player/Sprite")` with relative and absolute forms,
      `..` traversal, and a cached-lookup path so repeated resolution is not a per-frame string walk. **Dep:** 3.2
- [ ] **3.4 Lifecycle.** `Ready` / `Update(dt)` / `Exit` dispatched in deterministic tree order.
      **Direct synchronous calls — no message queue.** **Dep:** 3.2
- [ ] **3.5 Transform propagation.** Dirty-flagged world transform update, single flat pass over
      the node array. **Dep:** 3.1, 1.5

## Phase 4 — Resources

- [ ] **4.1 Resource table.** Generational handle table with refcounting and typed slots. **Dep:** 1.2
- [ ] **4.2 Loaders.** Texture (PNG via an MIT/Zlib decoder) and text/script assets. **Dep:** 4.1
- [ ] **4.3 Hot reload.** Filesystem watch, reload in place behind the existing handle so live
      references stay valid. **Dep:** 4.2

## Phase 5 — 2D Renderer

- [ ] **5.1 Sprite batcher.** Sort by (layer, texture), single dynamic vertex buffer, zero
      per-frame heap allocation. **Dep:** 2.5, 4.2
- [ ] **5.2 Camera2D.** View/projection, viewport letterboxing, screen<->world conversion. **Dep:** 1.5
- [ ] **5.3 `SpriteNode`.** First concrete node type; proves the scene→render path end to end. **Dep:** 3.5, 5.1

## Phase 6 — Scripting (AngelScript)

- [ ] **6.1 Vendor AngelScript.** Zlib-licensed, compatible with §1. Record in `thirdparty/README.md`. **Dep:** 0.1
- [ ] **6.2 Script engine wrapper.** Context pooling, compile/error reporting routed to the logger. **Dep:** 6.1, 1.3
- [ ] **6.3 Binding registry.** Type-safe registration of C++ handles into AngelScript. `Handle<T>`
      must round-trip without exposing a raw pointer to script. **Dep:** 6.2, 1.2
- [ ] **6.4 Bind core API.** `Vec2`, `Transform2D`, `Node`, `GetNode` / `$` sugar, logging. **Dep:** 6.3, 3.3
- [ ] **6.5 Script components.** One script per node, `Ready`/`Update`/`Exit` bound to the lifecycle
      from 3.4. Direct calls only. **Dep:** 6.4, 3.4
- [ ] **6.6 Script hot reload.** Recompile and reattach preserving node state where possible. **Dep:** 6.5, 4.3

## Phase 7 — Serialization & Compatibility

- [ ] **7.1 Format spec.** Write `docs/format/scene.md` and `docs/format/project.md` *before*
      the code. Text-based, diff-friendly, version-stamped.
- [ ] **7.2 Writer/reader.** Round-trip a scene with no data loss. Unknown fields are preserved,
      never dropped. **Dep:** 7.1, 3.2
- [ ] **7.3 Version guards.** Older → auto-backup then migrate step by step. Newer → refuse and
      report the engine version required. **Dep:** 7.2
- [ ] **7.4 Migration framework.** Append-only, ordered steps; released steps are never edited.
      Golden-file tests that load real files of every prior version. **Dep:** 7.3
- [ ] **7.5 Project file.** Project settings, asset roots, build targets. **Dep:** 7.2

## Phase 8 — Editor Shell

- [ ] **8.1 UI integration.** Vendor Dear ImGui (MIT) and/or RmlUi (MIT); render through
      `IRenderer` only — the editor must not touch a graphics API directly. **Dep:** 2.5
- [ ] **8.2 Editor app.** Window, dockspace, frame loop, layout persistence. **Dep:** 8.1, 2.6
- [ ] **8.3 Scene tree panel.** Hierarchy view, selection, drag-reparent. **Dep:** 8.2, 3.2
- [ ] **8.4 Inspector.** Property editing driven by reflected node data, not hand-written per type. **Dep:** 8.3
- [ ] **8.5 Viewport.** Render the scene into an editor texture; gizmos for select/move. **Dep:** 8.2, 5.3
- [ ] **8.6 Console.** Attach the 1.3 log sink; script errors clickable to source. **Dep:** 8.2, 6.2
- [ ] **8.7 Undo/redo.** Command stack over all scene mutation. Retrofitting this later is
      structurally painful — do it while the mutation surface is small. **Dep:** 8.4

## Phase 9 — Adaptive Platform UX

- [ ] **9.1 Interaction abstraction.** One intent model (`Primary`, `Context`, `Pan`, `Zoom`,
      `MultiSelect`) fed by both mouse and touch. **Dep:** 2.2, 8.2
- [ ] **9.2 Long-press action sheets.** Vertical, thumb-reachable, the mobile equivalent of a
      right-click context menu — same command set, different presentation. **Dep:** 9.1
- [ ] **9.3 Desktop context menus.** Right-click path over the same command set. **Dep:** 9.1
- [ ] **9.4 Gesture pan/zoom.** Pinch and two-finger pan in the viewport; scroll/middle-drag on desktop. **Dep:** 9.1, 8.5
- [ ] **9.5 Responsive layout.** Panel arrangement adapts to phone / tablet / desktop form factors. **Dep:** 8.2

## Phase 10 — AI Helper (BYOK)

- [ ] **10.1 HTTP client interface.** `IHttpClient` abstraction; platform backends behind it.
      TLS verification is never disabled. **Dep:** 1.3
- [ ] **10.2 OpenAI-compatible client.** Chat-completions request/response, streaming, timeouts,
      graceful error surfacing. **Dep:** 10.1
- [ ] **10.3 Provider config.** Base URL + model + key, with presets for Ollama, LM Studio,
      OpenRouter, OpenAI, Anthropic. **Dep:** 10.2
- [ ] **10.4 Key storage.** Platform-appropriate secure storage. Keys are **never** written into
      project files, logs, telemetry, or committed to Git. **Dep:** 10.3
- [ ] **10.5 Assistant panel.** Chat UI with editor context (selected node, open script). **Dep:** 10.3, 8.2

## Phase 11 — Mobile & Console Backends

- [ ] **11.1 Android backend.** NDK `IWindow`/`IInput`, GLES renderer, APK packaging. **Dep:** 2.6
- [ ] **11.2 iOS backend.** UIKit `IWindow`/`IInput`, Metal or GLES renderer, app bundle. **Dep:** 2.6
- [ ] **11.3 Metal device.** Second real `IRenderer` — the true test of whether 2.3 is actually
      abstract. **Dep:** 2.3
- [ ] **11.4 Console-readiness audit.** Verify no platform assumption leaks above the platform
      layer, so a closed-source backend can be added out-of-tree. **Dep:** 11.1, 11.2

## Phase 12 — Release

- [ ] **12.1 SemVer enforcement.** Version bump checklist; public-API diff gate in CI. **Dep:** 0.6, 0.3
- [ ] **12.2 `docs/CHANGELOG.md`.** Keep-a-Changelog format, updated per user-visible change.
- [ ] **12.3 Sample projects.** Two or three small games that exercise the full pipeline.
- [ ] **12.4 Getting-started docs.** Build instructions per platform; scripting primer.
- [ ] **12.5 License audit.** Confirm every vendored dependency is MIT/BSD/Zlib/Apache-2.0.
      No GPL/LGPL/AGPL anywhere in the tree. **Dep:** 12.3

---

## Open Decisions (need an ADR in `docs/adr/` before the dependent task starts)

- **D1 — Windowing strategy.** SDL3 (fast, one dependency, weaker console story) vs. native
  per-platform backends (more work, cleanest console path). Blocks 2.4.
- **D2 — Editor UI toolkit.** Dear ImGui (fast, immediate-mode, weak touch ergonomics) vs.
  RmlUi (retained, styleable, better for adaptive layout) vs. both. Blocks 8.1.
- **D3 — Scene file syntax.** Custom text format vs. TOML vs. JSON. Diff quality and
  migration ergonomics matter more than parse speed here. Blocks 7.1.
- **D4 — Reflection mechanism.** Macro-based registration vs. code generation. Drives the
  inspector (8.4) and serialization (7.2); decide once, live with it. Blocks 7.2 and 8.4.

## Follow-ups raised during Phase 0

- [ ] **F1 Clang-format config.** `AGENTS.md` §4 specifies a style; nothing enforces it
      mechanically yet. Add `.clang-format` and a CI format check.
- [ ] **F2 Sanitizer build.** An ASan/UBSan CI job. Cheap now, and the allocators in 1.4
      are exactly the code that needs it.
- [ ] **F3 Test-harness output for CI.** The harness prints human-readable results only.
      If CI annotation matters, add JUnit XML output.
- [ ] **F4 Per-test `ctest` registration.** Currently the whole suite is one `ctest` entry.
      Splitting it gives parallel execution and finer-grained failure reporting.

## Backlog (not scheduled)

- Audio subsystem (`IAudio` + backends)
- Physics / collision
- Tilemap node and editor tooling
- Particle system
- Animation (sprite sheet + tween)
- Localization
- Asset bundling / build pipeline for shipping games
- Nintendo Switch backend (out-of-tree, closed source)
