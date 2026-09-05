# DONE — Conduit Engine

Rolling summary of shipped work. Newest first.

> **Keep this file short.** It is a ledger, not a history book — `git log` is the history.
> See the pruning rules at the bottom and apply them *whenever you add an entry*.

**Current version:** `0.1.0-dev` (pre-alpha) · **Scene format:** `1` · **Project format:** `1`

---

## Unreleased — `0.1.0` (in progress)

### Phase 0 — Foundation *(complete)*
- **Buildable project skeleton.** CMake 3.20+ with `ConduitCore` / `ConduitRuntime` /
  `ConduitEditor` / `ConduitTests` targets, out-of-source enforced, C++17 baseline with a
  CI-tested C++20 opt-in. — *2026-09-05*
- **Warning and platform policy as build targets.** `Conduit::Warnings` and
  `Conduit::Platform` centralise strict warnings and the only OS detection in the tree, so
  no platform `#ifdef` can leak above `engine/platform/`. — *2026-09-05*
- **Version and file-format guards.** Generated `Conduit/core/Version.h` keeps the engine
  SemVer in sync with CMake and defines scene/project format versions as independent
  integers, with `ClassifyFormatVersion` implementing the load/migrate/refuse contract. — *2026-09-05*
- **Test harness and suite.** Dependency-free original framework (`tests/framework/Test.h`)
  wired to `ctest`; 9 tests covering the version guards and the harness itself. — *2026-09-05*
- **CI.** Build and test on Linux, macOS, and Windows across C++17/20 and Debug/Release,
  with warnings as errors. — *2026-09-05*
- **Project docs.** `README.md`, ADR process and the four open decisions, third-party
  license policy, `.gitignore`, `.editorconfig`. — *2026-09-05*

### Project setup
- **Agent documentation contract.** `AGENTS.md` (vision, non-goals, Git blackout policy,
  C++ standards, repo layout, definition of done), `CLAUDE.md` pointer, `TODO.md` phased
  roadmap through 1.0, and this ledger. — *2026-09-05*

---

## Pruning rules — apply on every edit

1. **One line per feature.** What shipped and why it matters. No implementation detail —
   that is what the commit and the code are for.
2. **Group under the release that contains it.** Work in flight goes under *Unreleased*.
3. **Hard cap: 200 lines.** When this file exceeds it, or when a release ships:
   collapse the oldest released section into a **single summary line**, and move its
   detail to `docs/CHANGELOG.md`.
4. **Keep at most three expanded sections**: *Unreleased*, the current release, and the
   one before it. Everything older is one line each.
5. **Never delete a version heading.** Collapse it; do not erase that it shipped.
6. Update the version/format line at the top when any of those three numbers change.
