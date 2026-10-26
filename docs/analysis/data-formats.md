# Rally Trophy (2001, Bugbear Entertainment) — Data Formats Analysis

Analysis date: 2026-10-22. Analyst: subagent shell-only analysis (xxd/python3/file/strings + web research).
All findings below were verified by decrypting and parsing **all 52 .bzf archives / 2933 entries** with
`tools/bzf_parse.py`; zero parse/decompression failures. Facts are tagged **[observed]** (verified on
bytes), **[inferred]** (strong circumstantial evidence), or **[external]** (from referenced sources).

## 1. Inventory

Root: `extracted/Program Files/JoWooD/Rally Trophy/Data/`

```
Data/
  Cars/car_1.bzf .. car_22.bzf        22 cars, ~2.8-3.4 MB each
  Drivers/driver1.bzf, driver2.bzf    animated driver/co-driver models
  Global/
    dynamics.bzf  (8.6 KB)            tire-vs-surface dynamics tables (5 x .ini)
    weather.bzf   (9.2 KB)            2 textures: RainDrops.tga, SnowFall.tga
    surfacesounds.bzf (4.3 KB)        10 per-surface sound .ini (grass, gravel, ...)
    fonts.bzf     (143 KB)            5 fonts: metrics blob + .tga glyph atlas each
    effects.bzf   (632 KB)            water/lake/river effect .ini + loop .tga
    flares.bzf    (617 KB)            sun/moon/lens flare .tga
    overlay.bzf   (447 KB)            HUD .tga (meters, countdown, on-screen bars)
  Tracks/track1 .. track5/
    data.bzf      (~200-280 KB)       stage config: .txt/.ini + .spf/.npf/.apf/.snd/.AI/.bin
    models.bzf    (41-57 MB)          track geometry + object models (.bmf, .tga)
    textures.bzf  (14-38 MB)          track textures (.tga)
  menu.bzf        (18 MB)             menu UI .tga + .ini
  soundfx.bzf     (7.1 MB)            3DSOUNDS.INI + ~environment .wav
  speech.bzf      (8 MB)              co-driver/pace notes _ENG/_GER .wav
  settings.bzf    (13 KB)             game.ini, video.ini, Camera.txt, cpu01-19.ini
  language.bzf    (9.5 KB)            credits.txt, Language.ini, language_{deutsch,english}.txt
  shaders.bzf     (13 KB)             24 .sha shader scripts (text)
  Music/          (outside .bzf)      19 .mp3
  Video/          (outside .bzf)      22 .bik (Bink video)
```

Container census over all archives, by inner file extension **[observed]**:
`.tga` 1796, `.wav` 382, `.ini` 242, `.bmf` 236, `.txt` 58, `.bin` 42, `.spf` 42, `.snd` 34,
`.npf` 32, `.apf` 28, `.sha` 24, `.AI` 10, extensionless 5, `.sfk` 2.

## 2. The .bzf container ("bbzf" v2001.06.06)

Identified via [bfstool](https://github.com/xNyaDev/bfstool) (supports "BZF bbzf v2001.06.06 (Rally
Trophy)"). Format fully reverse-engineered and independently re-derived; two deviations from
bfstool's bzf2001 model were found in the shipped data (key-stream origin and payload restart value,
see 2.3/2.5).

### 2.1 Archive header **[observed]**

| Offset | Type  | Size | Meaning |
|--------|-------|------|---------|
| 0x00   | u32   | 4    | magic = 0x667A6262 ("bbzf") |
| 0x04   | u32   | 4    | version = 0x06062001 ("v2001.06.06") |
| 0x08   | u32   | 4    | file_count |
| 0x0C   | -     | -    | file_count x 53-byte FileHeader records (encrypted) |

### 2.2 File header record (53 = 0x35 bytes each) **[observed]**

| Offset in record | Type     | Size | Meaning |
|------------------|----------|------|---------|
| 0x00             | u8       | 1    | flags; bit0 = payload is zlib-compressed |
| 0x01             | u32 LE   | 4    | data_offset (absolute payload offset in archive) |
| 0x05             | u32 LE   | 4    | unpacked_size — EXCLUDES the 43-byte copyright prefix (see 2.5) |
| 0x09             | u32 LE   | 4    | packed_size (actual stored bytes; entries are stored back-to-back: offset[i+1] = offset[i] + packed_size[i]) |
| 0x0D             | char[40] | 40   | file name, NUL-padded (0x28) |

### 2.3 XOR encryption **[observed]**

* Directory (file headers): each byte at absolute offset `a` (a >= 0x0C) is XORed with
  `KEY[a % 256]`. The 12-byte archive header is NOT encrypted.
* Payload: at each entry's `data_offset` the key-stream position restarts at **12 (0x0C)** —
  i.e. payload byte `i` of an entry is XORed with `KEY[(12 + i) % 256]`. (bfstool's model restarts
  at 0; Rally Trophy data restarts at 0x0C. Verified on all 2933 entries.)
* KEY is a fixed 256-byte table, identical for every archive in the game. It is embedded in
  `tools/bzf_parse.py`. It was recovered by known-plaintext analysis: name fields are NUL-padded,
  so most directory bytes are known-plaintext zero; per-key-position majority voting over all 52
  archives yields the full table, self-validated by clean parses of every archive.
* KEY visibly contains header-shaped fragments (e.g. `46 42 18 25 80 ... 6d 1d 78 e6`), i.e. it
  looks derived from sample file-header bytes **[inferred]**.

### 2.4 Payload storage **[observed]**

* flags bit0 set: payload = zlib stream (standard RFC 1950, e.g. starts `78 9c` after decrypt).
* flags bit0 clear: payload stored raw (observed for `.apf` stage files; e.g. stage1.apf
  packed=175 = 132+43).
* Compressed or not, EVERY payload begins with the 43-byte prefix
  `(c) 2001 by JoWooD Productions Software AG\0` — real content = 43 + unpacked_size bytes.
  The prefix is visible in plaintext in every decrypted payload (TGA files then start with a valid
  TGA header at byte 43, WAV with RIFF, etc.).

### 2.5 Hexdump excerpts (encrypted, as stored on disk)

`Cars/car_1.bzf` first 96 bytes (magic/version/count=0x2B=43, then encrypted directory):

```
00000000: 6262 7a66 0120 0606 2b00 0000 8a31 6180  bbzf. ..+....1a.
00000010: 4689 1825 80f8 aa02 6d28 78ba c16f 8e58  F..%....m(x..o.X
00000020: cb8d a630 3e83 1b09 4cce 18bb 7cae 6a30  ...0>...L...|.j0
00000030: 1e00 0ac9 24c0 98fd 33cc 0444 faea 8f33  ....$...3..D...3
00000040: c30c 6ae1 47d4 f0a8 8754 b4a8 5e17 70a7  ..j.G....T^.p.
00000050: 1b1d 64b2 9aa0 e713 ecb7 ff1e 0bd5 729a  ..d...........r.
```

`Global/dynamics.bzf` first 80 bytes (count=5):

```
00000000: 6262 7a66 0120 0606 0500 0000 8ad7 6880  bbzf. ........h.
00000010: 4642 2c25 80ba ac02 6d1d 78e6 cd31 a321  FB,%....m.x..1.!
00000020: a5ec cb59 5df0 3560 22a7 18bb 7cae 6a30  ...Y].5\`"...|.j0
00000030: 1e00 0ac9 24c0 98fd 33cc 0444 faea 8f33  ....$...3..D...3
00000040: c30c 0eef 47d4 da9b c754 fdc1 5817 66a1  ....G....T..X.f.
```

Decrypted car_1 directory record 0 (bytes 0x0C..0x40), showing plaintext layout:

```
01 f3 08 00 00 cb 00 00 00 a5 00 00 00 61 69 2e  ....... ....ai.
69 6e 69 00 ... (padded)      -> flags=1, off=0x8F3, unpacked=203, packed=165, name "ai.ini"
```

## 3. Asset (payload) classes

After stripping the 43-byte prefix, payloads are ordinary files. Verified with `file`:
TGA = true-color Targa (e.g. Body.tga: RGBA 1024x1024x32), WAV = RIFF PCM 16-bit mono 22050 Hz.

### 3.1 Text/config family (`.ini`, `.txt`, `.snd`, `.sha`) — game parameters are here

All are Windows-INI/CFG-style text (`Key = value;` with `#` comments). These carry essentially the
entire gameplay/physics tuning surface:

* **Per car (`Cars/car_N.bzf`)** — see §4.
* **`Global/dynamics.bzf`**: `Tire0Dynamics.ini` .. `Tire4Dynamics.ini` — per-surface tire dynamics
  (tarmac, wet tarmac, oil gravel, gravel hard, ...). Each defines ~20 `DynNN*` parameter sets:
  RollingResistance, InducedDragCoeff, PneumaticTrail/Offset, Z/X/C stiffness, Z/X friction.
* **`Global/surfacesounds.bzf`**: one ini per surface (grass, gravel, gravel2, mud, oilgravel,
  sand, snow, snowbank, tarmac, wettarmac) — skid/roll sound behaviour per surface.
* **Tracks**: `StageN.txt` (ambient/sun colors, shadows, particle/skidmark colors, wildlife),
  `stageN.snd` (ambient sound loop + random event sounds), `stagetimesN.ini` (target split times
  per difficulty), `SoundSetN.ini`, `info.ini`.
* **`settings.bzf`**: `game.ini`, `video.ini`, `Camera.txt`, `cpu01.ini`..`cpu19.ini` (AI
  opponent definitions — driver names/car assignments **[inferred]**).
* **`language.bzf`**: UI strings (deutsch/english) + credits.
* **`shaders.bzf`**: 24 text `.sha` shader scripts (BodyAlphaCube, CarShader, DamagedWindow, Dirt...).
* **`fonts.bzf`**: per font a 5500-byte metrics blob (extensionless, e.g. `CounterFont`) + `.tga`
  atlas (`mpFont16/20/32`, `mpFont32Ariel`).

### 3.2 `.bmf` — binary model format (Bugbear Mesh File) **[observed, partially parsed]**

Chunked binary; written by "3ds Max R3 Exporter" (string present in INFO chunk). Layout:
repeating `[u32 size][tag, 4 chars + NUL][payload]`; chunks nest (MAIN > INFO / MATERIALLIST >
MATERIAL / geometry lists). First u32 of MAIN equals the payload's full unpacked size. MATERIAL
payloads contain float32 quad(s) (0x3F800000 = 1.0f) — diffuse/specular colors **[inferred]**.
Used for car bodies (`model.BMF`), collision meshes (`collision.BMF`), wipers, rain drops, driver
body parts, and all track geometry. Not fully reverse-engineered (open question §6).

### 3.3 Track binary formats (in `Tracks/trackN/data.bzf`)

| Ext | Size pattern | Observed structure |
|-----|--------------|--------------------|
| `.spf` | 300-350 B | u8 0x00, then u32 x3 (=49,100,18 for stage1), then float32 triples — spline/path points **[inferred]** (probably "stage path file") |
| `.npf` | 1-1.8 KB | u32 0x1234?, u32 100, u32 count, then binary records — likely checkpoint/reset node path **[inferred, mostly unknown]** |
| `.apf` | stored raw | binary, ~108-132 B — likely animation path **[inferred]** |
| `.AI` | 3.5-5.7 KB | begins u32 0x40=64 then float32 triples — AI driving line / opponent data **[inferred]** |
| `resetN.bin` | ~10-38 KB | u32 count (e.g. 493), then count x 72-byte records (float32 quads: position + quaternion + ...) — checkpoint respawn transforms **[observed/inferred]** |

## 4. One car in detail: `Cars/car_1.bzf` (43 entries)

car_1 = **Mini Cooper S, 1964** (`info.ini`: Name "Mini", 1275 cc, 4-cyl, 100 hp, 670 kg).
Entry breakdown **[observed]**:

* 6 physics/config INI: `car.ini` (6.1 KB), `Engine.ini` (2.1 KB), `Tires.ini`, `ai.ini`, `Sounds.ini`, `info.ini`
* 4 BMF meshes: `model.BMF` (172 KB packed — visual body), `collision.BMF` (1 KB — physics
  collision mesh), `rain_drop.BMF`, `wipers.BMF`
* 25 TGA textures: body, interior hi/lo, windows (+damaged), lights (+damaged/lit/braking), dirt,
  steering wheel, dashboard meters, 2 overlay speedometers (KmH/Mph), info photos, flares
  (brake/front/rear/reverse), tire
* 8 engine WAV loops: `ME_*` / `MX_*` at idle/2k/4k/6k rpm (ME = engine, MX = exhaust
  **[inferred]**; cross-check with `Sounds.ini`)

### 4.1 Where the vehicle physics live (`car.ini`) **[observed]**

* Rigid body: `Mass = 670.0`, `MomentOfInertia = {0.8847, 0.9737, 0.2566}`,
  `CenterOfMass = {0, 0.3, 0.15}`, `AeroDrag`, `DownForceConst`, `Damping`
* Brakes: `BrakeBalance = {0.6, 0.525, 0.45}` (per setup), `BrakeTorque = 4000`,
  `HandBrakeTorque = 2500`
* Suspension: min/max length 0.03/0.35, rest length {soft,default,hard} = {0.30,0.25,0.20},
  spring const {0.8, 35000, 1.20} N/m (front) / 30000 (rear), roll bars, bump/rebound dampers,
  bump rubbers, camber
* Steering/tires: `TireTurnAngleIn/Out`, steering speed limits, tire width/radius/mass
* Damage: `DamageSpeedRange`, hood/trunk damage angles; wipers; camera/driver positions;
  force-feedback params; flare parameters

### 4.2 Drivetrain (`Engine.ini`) **[observed]**

`NumGears = 4` with per-gear `GearNRatio..Gear4Ratio = {short-mult, ratio, long-mult}` e.g.
gear1 `{1.10, 3.428, 0.90}`, `EndRatio = 4.05`, differential lock 0.2, clutch/shift times,
`TorqueMax = 115` Nm with a 17-point `TorqueNN = {rpm, fraction}` curve, `IdleRpm = 1200`.

### 4.3 AI driver (`ai.ini`) **[observed]**

Driver name/nationality + braking/oversteer/slowdown personality scalars. The game-wide AI
opponents presumably come from `settings.bzf` `cpuNN.ini` (to be confirmed, open question).

## 5. Parser

`tools/bzf_parse.py` (stdlib only):

```
python3 tools/bzf_parse.py list   <file.bzf>           # list entries
python3 tools/bzf_parse.py extract <file.bzf> <outdir> # decrypt + unpack all entries
python3 tools/bzf_parse.py dump   <file.bzf> <name>    # print one entry
```

Extracted files retain the 43-byte copyright prefix (strip with `tail -c +44`); the parser enforces
the `unpacked_size + 43` and zlib-integrity checks. It can also be imported (`parse_headers`,
`extract_entry`, `KEY`) for further tooling. Validated over all 52 archives / 2933 entries.

**Rust port:** `crates/rt-assets/src/bzf.rs` (`rt_assets::bzf`, `flate2`/`miniz_oxide` for zlib)
with the same semantics, plus `rally-trophy bzf list|extract|dump`. Verified 2026-10-22:
identical listing output for all 52 archives and byte-identical extracted payloads
against the Python reference.

## 6. Confidence summary & open questions

**Observed (byte-verified across all archives):** container header/record layout, XOR key and key
scheduling, zlib/raw payloads, 43-byte copyright prefix, entry contiguity, all per-car/track/global
asset listings, INI contents quoted above.

**Open questions:**
1. `.bmf` chunk grammar beyond MAIN/INFO/MATERIALLIST/MATERIAL (vertex/index encoding, coordinate
   system, node names for parts like wheel_fl — names not stored in .bzf, likely inside BMF).
2. Track `.npf`/`.apf` semantics and the exact record layout of `resetN.bin` (72-byte records,
   likely position + quaternion + metadata) and `.AI` floats (64-byte header then float triples).
3. Whether `cpuNN.ini` in settings.bzf are the AI opponents (very likely, unverified).
4. Why the payload key restarts at 0x0C rather than 0 (tooling quirk in Bugbear's packer —
   cosmetic, documented).
5. Exact meaning of `.spf` header ints (49/100/18) — point count, road width, flags?
6. The 5 extensionless font metric blobs and 2 `.sfk` files (likely soundforge peak data).
7. Whether other regional releases use a different XOR key (this key covers the retail EN/DE data
   on this disc).
