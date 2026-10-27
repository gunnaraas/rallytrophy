# Documentation index

This folder contains readable analysis and architecture notes. Function-level
matching maps, compiler notes, and the active work queue are under
[`analysis/`](../analysis/README.md).

## Start here

- [Architecture and scope](architecture/overview.md): what the decomp covers
  and how the matching workflow fits together.
- [Control-flow overview](analysis/control-flow.md): startup and major game
  paths.
- [Shipped documentation inventory](analysis/shipped-docs.md): manuals and
  text files found with the game.
- [String inventory](analysis/strings.md): strings grouped by subsystem.
- [Data formats](analysis/data-formats.md): observed game file structures.

## Subsystem analysis

- [File and archive handling](analysis/file-archive-cache.md),
  [buffer behavior](analysis/file-buffer.md), and
  [file opening](analysis/file-open.md).
- [Input initialization](analysis/input-initializers.md),
  [keyboard deque](analysis/keyboard-deque.md),
  [mouse position](analysis/mouse-position.md), and
  [force feedback](analysis/directinput-force-effect.md).
- [Scene objects](analysis/scene-object.md),
  [bounding spheres](analysis/bounding-sphere.md),
  [view objects](analysis/view-object.md), and
  [HUD geometry](analysis/hud-geometry.md).
- [Car visibility](analysis/car-cockpit-visibility.md),
  [suspension setters](analysis/car-suspension-setter.md),
  [drivetrain gear selection](analysis/drivetrain-select-gear.md),
  [skid-mark rendering](analysis/skid-render.md),
  [car rain setup](analysis/car-rain-setup.md),
  [particle emitter rendering](analysis/car-emitter-b-render.md), and
  [exhaust smoke](analysis/exhaust-smoke-emission.md).
- [Menu drawing](analysis/menu-row-drawing.md),
  [menu toggles](analysis/menu-toggle.md),
  [menu volume](analysis/menu-volume.md),
  [player-name plate drawing](analysis/player-name-plate.md),
  [player-name hit rectangles](analysis/player-name-build-rects.md),
  [service-menu geometry](analysis/service-menu-pages.md), and
  [menu setup lifetimes](analysis/menu-setup-lifetimes.md).
- [Networking session](analysis/networking-session.md),
  [configuration validation](analysis/config-validation.md),
  [language parsing](analysis/language-parser.md), and
  [WinMain startup](analysis/winmain-startup.md).

## Byte-match examples

[Selected exact-match observations](analysis/decomp-byte-matches.md) describe
compiler-sensitive instruction sequences and their source implementations.
The complete set of function maps is in `analysis/range-*.md`.
