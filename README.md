# Conduit

A lightweight, high-performance **2D game engine and cross-platform editor**, MIT licensed.

> **Status: pre-alpha (`0.1.0-dev`).** The foundation builds and tests cleanly on Linux,
> macOS, and Windows. There is no renderer, scene graph, or scripting yet — see
> [`TODO.md`](./TODO.md) for exactly what is next.

## What Conduit is

Defold's runtime frugality with Godot's authoring ergonomics, minus each engine's
biggest friction point:

- **No asynchronous message passing.** Defold's `msg.post` model is explicitly rejected.
  Node communication is direct, synchronous, and statically typed.
- **One script per node**, Godot-style, with direct path lookups: `GetNode("Player/Sprite")`.
- **AngelScript** for scripting — C/C++-style syntax, static typing, type-safe bindings
  to C++ engine handles.
- **One editor everywhere.** Native on iOS, Android, macOS, Windows, and Linux, drawn with
  a hardware-accelerated in-engine UI. Touch-first, adapting cleanly to mouse.
- **Portable by construction.** OS windowing, rendering, and input sit behind `IWindow`,
  `IRenderer`, and `IInput`, so a closed-source console backend drops in without touching
  engine code.
- **Your projects keep loading.** Strict SemVer 2.0.0, file format version guards,
  auto-backup, and forward migration.
- **Bring-your-own-key AI helper**, working against any OpenAI-compatible endpoint —
  local Ollama or LM Studio, OpenRouter, OpenAI, or Anthropic.

## Building

Requires **CMake ≥ 3.20** and a C++17 compiler (GCC 9+, Clang 10+, MSVC 2019+).

```sh
cmake -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
ctest --test-dir build --output-on-failure
```

Binaries land in `build/bin/`:

```sh
./build/bin/conduit          # runtime (game player)
./build/bin/conduit-editor   # editor
./build/bin/conduit-tests    # test suite; pass a substring to filter
```

### Build options

| Option | Default | Effect |
| --- | --- | --- |
| `CONDUIT_CXX_STANDARD` | `17` | Language standard; `20` is supported and CI-tested |
| `CONDUIT_BUILD_EDITOR` | `ON` | Build the editor |
| `CONDUIT_BUILD_RUNTIME` | `ON` | Build the standalone runtime |
| `CONDUIT_BUILD_TESTS` | `ON` | Build the test suite |
| `CONDUIT_WARNINGS_AS_ERRORS` | `OFF` | `-Werror` / `/WX`; CI always sets this `ON` |

## Repository layout

```
engine/     core, math, platform abstraction, render, scene, script, resource, serialize
editor/     editor shell, UI, panels, adaptive input, AI assistant
runtime/    standalone game player
tests/      test suite and its dependency-free harness
docs/       architecture decision records and file format specs
```

## Contributing

Read **[`AGENTS.md`](./AGENTS.md)** first — it is the contract for humans and AI agents
alike: vision and non-goals, C++ standards, performance rules, the Git commit blackout
policy, and the definition of done.

Then take the next unblocked task from **[`TODO.md`](./TODO.md)**.
Shipped work is summarised in **[`DONE.md`](./DONE.md)**.

## License

MIT. See [`LICENSE`](./LICENSE). All code is original; every vendored dependency must be
MIT/BSD/Zlib/Apache-2.0 and recorded in [`thirdparty/README.md`](./thirdparty/README.md).
