# DONE — Conduit Engine

Rolling summary of shipped work. Newest first.

> **Keep this file short.** It is a ledger, not a history book — `git log` is the history.
> See the pruning rules at the bottom and apply them *whenever you add an entry*.

**Current version:** `0.0.0` (pre-alpha) · **Scene format:** `1` · **Project format:** `1`

---

## Unreleased — `0.1.0` (in progress)

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
