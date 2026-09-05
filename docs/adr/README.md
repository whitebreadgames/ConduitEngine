# Architecture Decision Records

An ADR captures a decision that is expensive to reverse: why it was made, what was
rejected, and what it commits us to. Conduit keeps them because the reasoning behind a
structural choice outlives the person who made it.

## When to write one

Write an ADR before starting any task in `TODO.md` that names an open decision (D1–D4),
and any time you pick between approaches where the loser was genuinely viable.

## Format

One file per decision, `NNNN-short-title.md`, numbered in order:

```markdown
# ADR NNNN — Title

**Status:** Proposed | Accepted | Superseded by ADR NNNN
**Date:** YYYY-MM-DD

## Context
What forces are in play. What we know, what we do not.

## Decision
What we are doing, stated plainly.

## Alternatives considered
Each option, and the specific reason it lost. An alternative with no stated
weakness was not seriously considered.

## Consequences
What this makes easy, what it makes hard, and what it locks us into.
```

Records are append-only. A decision that turns out wrong gets a new ADR that supersedes
the old one; the original stays, marked superseded.

## Open decisions awaiting an ADR

| ID | Decision | Blocks |
| --- | --- | --- |
| D1 | Windowing strategy: SDL3 vs. native backends | TODO 2.4 |
| D2 | Editor UI toolkit: Dear ImGui vs. RmlUi vs. both | TODO 8.1 |
| D3 | Scene file syntax: custom text vs. TOML vs. JSON | TODO 7.1 |
| D4 | Reflection: macro registration vs. code generation | TODO 7.2, 8.4 |
