# AGENTS.md — Conduit Engine Contributor Contract

> **Read this file in full before writing a single line of code.**
> It is the canonical brief for every human or AI agent working on Conduit.
> `CLAUDE.md` is a pointer to this file. If guidance conflicts, **this file wins**,
> except where a rule here is overridden by an explicit instruction from the project owner.

---

## 1. Project Vision

**Conduit** is a lightweight, high-performance, open-source **2D game engine and
cross-platform editor** written in modern C++.

The design thesis: *Defold's runtime frugality, Godot's authoring ergonomics, without
either engine's biggest friction point.*

| Pillar | Position |
| --- | --- |
| **Runtime cost** | Fast, low-overhead C++ engine loop. Frame budget is sacred. |
| **No async messaging** | Defold's `msg.post` model is **explicitly rejected**. Communication is direct, synchronous, and type-checked. |
| **Node model** | Godot-style **single script per node**, with direct string path lookups: `GetNode("Player/Sprite")` / `$Player/Sprite`. |
| **Scripting** | **AngelScript** — C/C++-style syntax, static typing, explicit type-safe bindings to C++ engine handles. |
| **Editor reach** | One editor, native on **iOS, Android, macOS, Windows, Linux**, drawn with an in-engine hardware-accelerated UI (Dear ImGui / RmlUi over OpenGL/Metal/Vulkan). |
| **Adaptive UX** | Touch-first mobile (long-press vertical action sheets, gesture pan/zoom) that adapts cleanly to desktop mouse + right-click context menus. |
| **Portability** | OS windowing, rendering, and input sit behind clean C++ interfaces (`IWindow`, `IRenderer`, `IInput`) so closed-source console backends (e.g. Nintendo Switch) can be dropped in without touching engine code. |
| **Safety of user data** | Strict **SemVer 2.0.0**. Project/scene files carry format version guards with auto-backup and migration paths. Asset corruption is treated as a P0 defect class. |
| **AI assistance** | Embedded OpenAI-compatible **BYOK** helper: local Ollama / LM Studio, free OpenRouter endpoints, or paid OpenAI/Anthropic keys — all through one standardized HTTP client. |
| **License** | 100% original code, **MIT**. |

### Non-goals (say no to these)
- Asynchronous message passing between nodes (`msg.post` and equivalents).
- 3D rendering. Conduit is 2D. Depth is a sort key, not a dimension.
- A scripting language other than AngelScript in the core runtime.
- Vendored GPL/LGPL/AGPL code, or any dependency that is not MIT/BSD/Zlib/Apache-2.0 compatible.
- Editor features that only work on desktop. If it cannot be driven by a finger, it is not done.

---

## 2. MANDATORY GIT COMMIT BLACKOUT (CRITICAL)

To protect personal IP and stay compliant with employment/workplace agreements, a Git
write blackout is enforced.

### The window
**Monday–Friday, 06:00–16:00 local time.**

### Rules of engagement
1. **NEVER** generate, execute, or suggest commands that write Git history during the
   window. This includes but is not limited to:
   `git commit`, `git push`, `git rebase`, `git merge`, `git cherry-pick`, `git revert`,
   `git tag`, `git am`, `git apply` to history, release scripts, and PR creation/merge.
2. If asked to commit, push, or cut a release inside the window: **refuse gently**,
   restate the policy, and offer the permitted alternatives.
3. Outside the window — weekdays after 16:00, before 06:00, and **all day Saturday and
   Sunday** — normal Git operation is fully allowed.

### Always permitted, any time
`git status`, `git diff`, `git log`, `git add` (staging), `git stash`, `git branch`
(listing), reading files, editing the working tree, building, and running tests.

### Required pre-flight
Before any history-writing command, **check the local date and time first**
(e.g. `date`) and state the verdict out loud. Never assume. Never rely on a timestamp
read earlier in the session — sessions outlive the window boundary.

```
$ date
Sat Sep  5 12:07:04 UTC 2026   -> Saturday, weekend, Git writes ALLOWED
Tue Sep  8 09:30:00 UTC 2026   -> Weekday 09:30, BLACKOUT, stage or stash only
Tue Sep  8 17:05:00 UTC 2026   -> Weekday 17:05, after 16:00, Git writes ALLOWED
```

---

## 3. Branch & Commit Policy

- Development branch for agent work: **`claude/conduit-engine-architecture-j1u6mw`**.
  Create it locally if absent. Never push to a different branch without explicit permission.
- Push with `git push -u origin <branch>`. On network failure, retry up to 4 times with
  exponential backoff (2s, 4s, 8s, 16s).
- **Do not open a pull request unless explicitly asked.**
- Commit messages: imperative mood, scoped prefix, wrapped at 72 columns.
  ```
  core: add handle-based ResourceTable with generational slots

  Slots carry a 16-bit generation counter so a stale Handle<T> resolves to
  nullptr instead of a recycled object. Lookup is a single array index.
  ```
  Accepted scopes: `core`, `math`, `platform`, `render`, `scene`, `script`,
  `resource`, `serialize`, `editor`, `ai`, `runtime`, `build`, `docs`, `tests`, `ci`.
- Never put a model identifier or marketing model name in a commit message, PR body,
  code comment, or any other artifact pushed to the repository.

---

## 4. Code & Architecture Standards

### Language and tooling
- **C++17 minimum, C++20 where every target toolchain supports it.** No compiler-specific
  extensions in portable code; isolate them in the backend that needs them.
- Build system: **CMake ≥ 3.20**, out-of-source, targets over directory-scoped flags.
- Warnings: `-Wall -Wextra -Wpedantic` (`/W4` on MSVC), and **warnings are errors in CI**.

### Performance rules (non-negotiable in the frame loop)
- **No dynamic allocation in the per-frame hot path.** Allocate up front, reuse, or use a
  frame arena that resets each tick.
- **Cache-friendly layout.** Prefer contiguous arrays over pointer-chasing graphs. Hot and
  cold data live in separate structures.
- **Handle-based resource management.** Public APIs hand out opaque `Handle<T>`
  (index + generation), never raw or shared pointers to engine-owned objects.
- No RTTI and no exceptions across the engine ABI boundary. Errors are returned, not thrown.
- No virtual dispatch in inner loops. Virtual is for hardware abstraction seams, where the
  call happens once per frame or once per resource — not once per sprite.
- Measure before optimizing, and measure after. A claimed speedup without a number is noise.

### Style
- Idiomatic modern C++. Clear header/source separation; headers stay light.
- Everything lives in `namespace Conduit`, with nested namespaces per subsystem
  (`Conduit::Platform`, `Conduit::Scene`, `Conduit::Script`).
- Naming: `PascalCase` types and functions, `camelCase` locals and parameters,
  `m_` prefix on private members, `k` prefix on compile-time constants,
  `SCREAMING_SNAKE_CASE` reserved for macros only.
- Include order: own header, then C++ standard library, then third-party, then Conduit
  headers — each block alphabetized, separated by a blank line.
- 4-space indent, no tabs. Braces on their own line for types and functions.
- Comments explain **why**, never **what**. Match the density of surrounding code.

### Architecture patterns
- Hardware abstraction is **pure interface**: `IWindow`, `IRenderer`, `IInput` are abstract
  classes with zero platform types in their signatures. No `#ifdef _WIN32` above the
  platform layer, ever.
- Data-driven wherever the data outlives the code that reads it (scenes, resources,
  editor layout, input maps).
- Engine core must never include an editor header. The dependency arrow points one way:
  `editor -> engine`, never back.

---

## 5. Repository Layout (target)

```
ConduitEngine/
├── CMakeLists.txt
├── cmake/                 # toolchain files, platform detection, package helpers
├── engine/
│   ├── core/              # types, Handle<T>, allocators, logging, assertions
│   ├── math/              # Vec2, Mat3, Rect, Transform2D
│   ├── platform/
│   │   ├── IWindow.h  IRenderer.h  IInput.h
│   │   └── backends/      # win32, x11, wayland, cocoa, uikit, android
│   ├── render/            # sprite batcher, camera, GL/GLES/Metal/Vulkan devices
│   ├── scene/             # Node, NodeTree, path resolution, lifecycle
│   ├── script/            # AngelScript VM, binding registry, script components
│   ├── resource/          # handle table, loaders, hot reload
│   └── serialize/         # project/scene format, version guards, migration
├── editor/
│   ├── app/               # editor entry point and shell
│   ├── ui/                # ImGui/RmlUi integration layer
│   ├── panels/            # scene tree, inspector, viewport, console
│   ├── input/             # adaptive touch/desktop interaction model
│   └── ai/                # BYOK OpenAI-compatible assistant
├── runtime/               # standalone game player
├── tests/                 # unit and integration tests
├── thirdparty/            # vendored MIT/BSD/Zlib dependencies only
└── docs/                  # architecture notes, format specs, ADRs, CHANGELOG
```

---

## 6. Versioning & File Format Compatibility

- The engine follows **SemVer 2.0.0** exactly. Pre-1.0, the minor version carries breaking
  changes; the API is explicitly unstable until 1.0.0.
- Every serialized project and scene file begins with a format version integer,
  independent of the engine version.
- On load: version equal -> proceed. Version older -> **back up the original file, then
  migrate through each intermediate step in order.** Version newer -> refuse to load and
  report the required engine version. Never silently discard unknown fields.
- Migration steps are append-only and are never edited once released.
- Any change to a serialized format requires: a version bump, a migration step, a test
  that loads a real file of the previous version, and a note in `docs/CHANGELOG.md`.

---

## 7. Working Agreement for Agents

1. **Start here.** Read `AGENTS.md`, then `TODO.md`, then `DONE.md`. `TODO.md` is the
   single source of truth for what happens next.
2. **Check the clock before any Git write.** See §2.
3. **Take the next unblocked task** from `TODO.md` unless directed otherwise. Do not skip
   ahead to a task whose dependencies are unbuilt.
4. **Keep the scope you were given.** Don't silently widen, narrow, or transform a task.
   If a task is wrong, say so in a sentence, then deliver it under stated assumptions.
5. **Build and test before committing.** A push that breaks the build costs everyone.
6. **Update the ledgers in the same commit as the work.** Move the finished item out of
   `TODO.md` and add a one-line entry to `DONE.md`. Add any newly discovered follow-up
   work to `TODO.md` rather than leaving it in your head.
7. **Report honestly.** If tests fail, show the output. If a step was skipped, name it.
8. **Never vendor a dependency** without checking its license against §1 non-goals and
   recording it in `thirdparty/README.md`.

---

## 8. Definition of Done

A task is done when **all** of these hold:
- [ ] It compiles clean with warnings-as-errors on at least one desktop toolchain.
- [ ] It has tests, or a written reason why it cannot be tested.
- [ ] It introduces no platform `#ifdef` above the platform layer.
- [ ] It allocates nothing per-frame in the hot path.
- [ ] Public headers are documented at the intent level.
- [ ] `TODO.md` and `DONE.md` are updated.
- [ ] The commit message follows §3.
