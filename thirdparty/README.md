# Third-party dependencies

Conduit ships under the MIT license and every vendored dependency must be
license-compatible: **MIT, BSD, Zlib, or Apache-2.0 only**. GPL, LGPL, and AGPL code
is not permitted anywhere in this tree (`AGENTS.md` §1).

Record every dependency here when you vendor it — name, version, license, upstream
URL, and why it is here. An unlisted dependency fails the license audit (TODO 12.5).

## Currently vendored

*Nothing yet.*

The test harness is deliberately original code (`tests/framework/Test.h`) rather than a
vendored framework: it is small enough to own, has no version skew, and compiles on
every console toolchain Conduit will need to meet.

## Planned

| Dependency | License | Purpose | Blocking task |
| --- | --- | --- | --- |
| AngelScript | Zlib | Scripting runtime | TODO 6.1 |
| Dear ImGui and/or RmlUi | MIT | Editor UI | TODO 8.1, decision D2 |
| SDL3 *(if D1 selects it)* | Zlib | Desktop windowing/input | TODO 2.4, decision D1 |
| PNG decoder | MIT/Zlib | Texture loading | TODO 4.2 |
