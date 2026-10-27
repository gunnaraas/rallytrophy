# Track-book quad drawing

`TrackSelectMenu::DrawTrackBook` at `0x445c90` is a 434-byte thiscall
function with an EBP frame, SEH handler `0x4d954e`, and 0x4c bytes of locals.
REA evidence
`ev_d75f1466e03adee2022eecff0abdd168bae0f3e3d71c762a1d5c090fe91daef3`
confirms the target body and its texture call, four-point position and UV
arrays, color conversion, quad call, and cleanup.

Two coordinate values use a target-specific x87 spill sequence. For the
shared right-edge X value, the target stores the x87 value without popping,
copies its bits through EAX, then stores the same value to two points. The
bottom-edge Y value follows the same pattern through ECX. The current source
candidate is 422 bytes with 140 differing bytes outside relocations.

A temporary named-float form for the two shared coordinates emitted the same
422-byte candidate. Volatile named floats grew it to 440 bytes and 153
differences, while also shifting the color-conversion temporaries. The
tracked function body remains unchanged. These probes do not reproduce the
target spill schedule.
