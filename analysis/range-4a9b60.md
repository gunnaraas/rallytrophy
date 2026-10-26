# Range 0x4a9b60-0x4b27f3 (73 fns in functions.csv; none has a src/ FUNCTION/NONMATCHING marker). Observation from pseudocode + header grep; names are reconstructions.
Note: first inventory fn is 0x4aa430 (0x4a9b60-0x4aa42f has none).

| Range | Role | Vtable |
|---|---|---|
| 0x4aa430-0x4aa9c0 | Font/text helpers: GetWidth 4aa430 (header: float GetWidth(const char*,int)), 4aa460, 4aa5a0, 4aa690 (816B) ; call 4c2414 | none |
| 0x4aa9c0-0x4aaca0 | Small stubs/dtor-ish: 4aa9c0 (508B), 4aabd0, 4aabf0 (->4c0d01), 4aac50 | none seen |
| 0x4aaca0-0x4aafc0 | VidImage (0x20B raw pixel buffer, "Out of memory"): fastcall stubs 4aaca0/4aacd0, ctor 4aad10, SessionObject ctor 4aae80, dtor 4aafa0, 4aafc0 | none (PTR_s_Out_of_memory 0x50216c) |
| 0x4ab010-0x4ac920 | Image loader/decoder (probably BMP/TGA/PCX or JPEG/PNG; "Out of memory"): 4ab010 (2302B), 4ab910 (1523B), 4abf30 (2034B, stdcall, calls 4c1290 + 4a4e00/4a5fa0), 4ac730 Save(file) (488B), 4ac920 (1080B) | none |
| 0x4acd60-0x4ad110 | Throwing resize/copy fn 4acd60 (933B, std::logic_error 0x4e2c78), 4ad110 (139B) | 4e2c78 (thrown) |
| 0x4ad1d0 | Single big cdecl box/geometry test: 4ad1d0 (3131B, float* a0,b0,a1,b1,a2,b2) | none |
| 0x4ade40-0x4ae4b0 | CarModelCache: ctor 4ade40, dtor 4ade80, 4adf50, Preload 4ae070, Release 4ae0e0, 4ae160, Reload 4ae1f0 (699B) | none seen (ctor stub) |
| 0x4ae4b0-0x4aecb0 | Model cache serialize/load: 4ae4b0 (1115B), 4ae910 (920B), both throw logic_error; 4aecb0 small | 4e2c78 (thrown) |
| 0x4aecf0-0x4aee00 | Intrusive doubly linked list w/ free list: AddTail 4aecf0, 4aed60 | none |
| 0x4aee00-0x4af270 | CarTextureCache / TextureLoader: ctor 4aee00, dtor 4aee20, SetDetail 4aeec0, 4aeef0, Load 4aef30, 4af030, 4af120, Upload 4af180, Count 4af1b0, Step 4af1c0 | none seen |
| 0x4af230-0x4af950 | Small cdecl helpers on 4a5e00/4a5f40 (ref-count/array accessors) and parsers: 4af230/250/490/4b0/500/560/710/730, 4af270 (464B), 4af870, 4af890 | none |
| 0x4af950-0x4b2330 | ModelFile (.bmf car part model, 0x80B stack object): ctor-ish 4af950 (1227B), dtor 4afe20, 4afec0 (693B), 4b0180 (1626B), 4b0920, 4b0a30, 4b0cd0, 4b0e90 (1328B), 4b13c0, 4b1640, 4b18b0, 4b19b0, 4b1af0 (970B), 4b1ec0 (963B), 4b2290, 4b2330 (505B); heavy callers of 4af230 | 0x4e1360 (stores in 4afec0, 4b0e90, 4b1af0, 4b1ec0, 4b2330) |
| 0x4b2530-0x4b27f3 | Scene/object init: 4b2530 (434B, calls 4bc680/4b2b30), 4b26f0 (259B); leads into 0x4b2830 base | not seen |

Skip/third-party candidates: 0x4ab010, 0x4ab910, 0x4abf30, 0x4ac920 likely a bundled image codec (pseudocode uses 4c1290/4c1276 CRT alloc); verify by strings before porting. 4bd000/4bd00b/4c0d01/4c1276/4c1290/4c11f4/4c2414 are CRT/new/delete outside range. Static-init stubs: none identified.

## Work groups (drafting)
1. 0x4aa430 0x4aa460 0x4aa5a0 0x4aa690 0x4aa9c0 0x4aabd0 0x4aabf0 0x4aac50
2. 0x4aaca0 0x4aacd0 0x4aad10 0x4aae80 0x4aafa0 0x4aafc0
3. 0x4ab010
4. 0x4ab910
5. 0x4abf30
6. 0x4ac730 0x4ac920
7. 0x4acd60 0x4ad110
8. 0x4ad1d0
9. 0x4ade40 0x4ade80 0x4adf50 0x4ae070 0x4ae0e0 0x4ae160 0x4ae1f0
10. 0x4ae4b0 0x4ae910 0x4aecb0
11. 0x4aecf0 0x4aed60 0x4aee00 0x4aee20 0x4aeec0 0x4aeef0 0x4aef30 0x4af030 0x4af120 0x4af180 0x4af1b0 0x4af1c0
12. 0x4af230 0x4af250 0x4af270 0x4af490 0x4af4b0 0x4af500 0x4af560 0x4af710 0x4af730 0x4af870 0x4af890
13. 0x4af950 0x4afe20
14. 0x4afec0 0x4b0920 0x4b0a30 0x4b0cd0
15. 0x4b0180
16. 0x4b0e90
17. 0x4b13c0 0x4b1640 0x4b18b0 0x4b19b0
18. 0x4b1af0 0x4b1ec0 0x4b2290 0x4b2330
19. 0x4b2530 0x4b26f0
