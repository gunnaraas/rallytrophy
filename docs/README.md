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
  [HUD geometry](analysis/hud-geometry.md) and
  [trail queue drawing](analysis/trailqueue-draw.md).
- [Parallel shadow rendering](analysis/parallel-shadow-render.md).
- [Car visibility](analysis/car-cockpit-visibility.md),
  [stage-point distance check](analysis/car-stage-point-distance.md),
  [steering geometry](analysis/car-steering-geometry.md),
  [suspension forces](analysis/car-suspension-forces.md),
  [reset and place on track](analysis/car-reset-place-track.md),
  [physics step](analysis/car-physics-step.md),
  [wheel contact](analysis/car-wheel-contact.md),
  [suspension setters](analysis/car-suspension-setter.md),
  [drivetrain gear selection](analysis/drivetrain-select-gear.md),
  [skid-mark rendering](analysis/skid-render.md),
  [car environment-map drawing](analysis/car-environment-map-draw.md),
  [car rain setup](analysis/car-rain-setup.md),
  [particle emitter rendering](analysis/car-emitter-b-render.md),
  [wheel-spray rendering](analysis/wheel-spray-render.md), and
  [exhaust smoke](analysis/exhaust-smoke-emission.md).
- [Menu drawing](analysis/menu-row-drawing.md),
  [menu toggles](analysis/menu-toggle.md),
  [menu volume](analysis/menu-volume.md),
  [sound-menu layout](analysis/sound-menu-layout.md),
  [track-menu layout](analysis/track-menu-layout.md),
  [main-menu layout](analysis/main-menu-layout.md),
  [race-options layout](analysis/race-options-layout.md),
  [modal confirmation](analysis/modal-confirmation.md),
  [modal result loop](analysis/modal-result-loop.md),
  [player-name menu setup](analysis/player-name-setup.md),
  [player-name plate drawing](analysis/player-name-plate.md),
  [player-name hit rectangles](analysis/player-name-build-rects.md),
  [track-book quad drawing](analysis/trackbook-draw.md),
  [service-menu geometry](analysis/service-menu-pages.md), and
  [menu setup lifetimes](analysis/menu-setup-lifetimes.md).
- [AI path segment selection](analysis/ai-driver-find-segment.md).
- [Car-model cache constructor](analysis/car-model-cache-constructor.md).
- [Scene-entry destructor thunk](analysis/scene-entry-destructor-thunk.md).
- [Texture upload helper](analysis/model-texture-upload.md).
- [Image lookup-table initializer](analysis/model-image-lut.md).
- [Mesh-colour helper](analysis/model-mesh-colour.md).
- [Networking session](analysis/networking-session.md),
  [configuration validation](analysis/config-validation.md),
  [language parsing](analysis/language-parser.md), and
  [WinMain startup](analysis/winmain-startup.md).

## Byte-match examples

[Selected exact-match observations](analysis/decomp-byte-matches.md) describe
compiler-sensitive instruction sequences and their source implementations.
The complete set of function maps is in `analysis/range-*.md`.
