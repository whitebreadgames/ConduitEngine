# CLAUDE.md

This project's contributor contract lives in **[`AGENTS.md`](./AGENTS.md)**.
Read it in full before making any change.

Quick orientation:
- **[`AGENTS.md`](./AGENTS.md)** — vision, non-goals, code standards, Git blackout policy, definition of done.
- **[`TODO.md`](./TODO.md)** — the single source of truth for what happens next. Take the next unblocked task.
- **[`DONE.md`](./DONE.md)** — rolling summary of shipped features. Keep it short; archive old cycles.

Two rules that are easy to miss and expensive to break:

1. **Git blackout window.** No `git commit`, `git push`, `git rebase`, `git merge`, `git tag`,
   or release scripts on **weekdays between 06:00 and 16:00 local time**. Run `date` and
   state the verdict *before* any history-writing command — never assume, and never reuse a
   timestamp read earlier in the session. Staging, stashing, editing, building and testing
   are always fine. See `AGENTS.md` §2.

2. **No asynchronous message passing.** Conduit deliberately rejects Defold's `msg.post`
   model. Node communication is direct, synchronous, and statically typed. See `AGENTS.md` §1.
