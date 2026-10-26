# Shipped-doc observations (non-REA)

Plain observations from files shipped on the original disc. These are **not**
REA Evidence; they corroborate analysis findings.

## System requirements (ReadMe.txt)

- Requires **DirectX 8.0** installed (confirms the D3D8 renderer stack
  discovered via `bbdx8.dll` → `d3d8.dll` import analysis).
- Genre self-description: "Rally Simulation", "handling and behaviour of a
  car is very realistic" — supports a careful physics port over an
  arcade approximation.

## Default controls (ReadMe.txt)

| Action | Key |
|---|---|
| Steer | Left/Right arrows |
| Accelerate / brake | Up/Down arrows |
| Handbrake | Space |
| Reverse gear | Ctrl + Up/Down |
| Shift up / down | A / Z |
| Rearview mirror | M |
| Toggle gauges | G |
| Reset car on track | R |
| Cycle cameras | C; cameras 1–6, extra replay 7–0 |
| Pause/exit | Esc |

Wheel users are told to enable "separate pedals" in Windows Gaming Options —
the input layer must support combined vs. separate pedal axes.

## Driving model hints (ReadMe.txt)

- Drivetrain layouts modeled distinctly: FWD (understeer on throttle),
  RWD (throttle oversteer), plus 4WD implied by car set.
- Techniques explicitly supported by the model: left-foot braking,
  handbrake turns, scandinavian flick — i.e. per-wheel grip/slip with
  weight transfer matters, not just a kinematic arcade model.

## Multiplayer

- Lobby executable shipped separately (`RallyTrophyLobby.exe`); GameSpy
  integration confirmed in binary strings (`key.gamespy.com`).
