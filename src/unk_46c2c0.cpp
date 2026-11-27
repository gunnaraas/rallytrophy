// Wheel (0x46c2c0-0x46d6ef, vtable 0x4e1ee8): one tyre of a Car. Map: analysis/range-46a8a0.md section 4.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include "unk_47c460.h"
#include "unk_46c2c0.h"
#include <math.h>
#include <string.h>

extern float DAT_004e1348;
extern float DAT_004e134c;
extern float DAT_004e138c;
extern "C" float fun_0046aff0();
extern "C" float fun_0046afb0();

// The Car's drivetrain (Car+0xdb8); only the two torque getters IntegrateSpin calls.
class WheelDrive {
public:
    float GetInertia(); // 0x46aff0: engine inertia reflected to the wheel
    float GetTorque();  // 0x46afb0: drive torque
};

// The Car as IntegrateSpin sees it: the drivetrain at +0xdb8.
struct WheelCar {
    BYTE unk0[0xdb8];
    WheelDrive drive;
};

// From the tyre-force draft:
// Wheel (0x46c2c0-0x46d6ef): the tyre force model. Map: analysis/range-46a8a0.md section 4.
// Only UpdateForces (0x46c790) and the Pacejka helper (0x46d590) are written here; the other functions of the file
// live in the same source in the original (address order).
typedef char AssertWheelLoad[offsetof(Wheel, load) == 0x2ac ? 1 : -1];
typedef char AssertWheelF32c[offsetof(Wheel, share) == 0x32c ? 1 : -1];
typedef char AssertWheelF264[offsetof(Wheel, unk264) == 0x264 ? 1 : -1];
typedef char AssertWheelF28c[offsetof(Wheel, slipB) == 0x28c ? 1 : -1];
typedef char AssertBodyCentre[offsetof(WheelBody, centre) == 0x224 ? 1 : -1];
typedef char AssertBodyTorque[offsetof(WheelBody, torque) == 0x1f0 ? 1 : -1];
extern float DAT_00509418;             // runtime friction scale (.data, written elsewhere)
extern float DAT_0050941c;             // runtime relaxation scale
static const float kMinLoad = 0.01f;   // 0x4e1edc
// The Pacejka "magic formula" (0x46d590, cdecl).
static float Pacejka(float x, float B, float C, float D, float E);
// x87 transform of the vector *point by the 3x3 part of *matrix (row vectors) plus *trans, written to *result; the
// original is hand-written assembler whose pointer operands live in stack locals.
#define WHEEL_XFORM_ADD() \
    __asm mov ecx, point \
    __asm mov edx, matrix \
    __asm mov eax, result \
    __asm mov ebx, trans \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+16] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+20] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+24] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+32] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+36] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+40] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fxch st(2) \
    __asm fadd dword ptr [ebx] \
    __asm fxch st(1) \
    __asm fadd dword ptr [ebx+4] \
    __asm fxch st(2) \
    __asm fadd dword ptr [ebx+8] \
    __asm fxch st(1) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+8] \
    __asm fstp dword ptr [eax+4]
// The same without a translation, outputs stored x, y, z.
#define WHEEL_XFORM() \
    __asm mov ecx, point \
    __asm mov edx, matrix \
    __asm mov eax, result \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+16] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+20] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+24] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+32] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+36] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+40] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fxch st(2) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+4] \
    __asm fstp dword ptr [eax+8]
// The same with the translation taken from the fourth row of the matrix.
#define WHEEL_XFORM_ROW() \
    __asm mov ecx, point \
    __asm mov edx, matrix \
    __asm mov eax, result \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+16] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+20] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+24] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+32] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+36] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+40] \
    __asm fxch st(2) \
    __asm faddp st(5), st \
    __asm faddp st(3), st \
    __asm faddp st(1), st \
    __asm fxch st(2) \
    __asm fadd dword ptr [edx+48] \
    __asm fxch st(1) \
    __asm fadd dword ptr [edx+52] \
    __asm fxch st(2) \
    __asm fadd dword ptr [edx+56] \
    __asm fxch st(1) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+8] \
    __asm fstp dword ptr [eax+4]
// Cross product *result = *a x *b as the original's hand-written x87 block.
#define WHEEL_CROSS() \
    __asm mov ecx, a \
    __asm mov edx, b \
    __asm mov eax, result \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx+8] \
    __asm fmul dword ptr [edx+4] \
    __asm fld dword ptr [ecx] \
    __asm fmul dword ptr [edx+8] \
    __asm fld dword ptr [ecx+4] \
    __asm fmul dword ptr [edx] \
    __asm fxch st(2) \
    __asm fsubp st(5), st \
    __asm fsubp st(3), st \
    __asm fsubp st(1), st \
    __asm fxch st(2) \
    __asm fstp dword ptr [eax] \
    __asm fstp dword ptr [eax+4] \
    __asm fstp dword ptr [eax+8]

// Inline store keeps y ahead of pop edi without volatile (a plain flat store sinks to the end of the ctor).
static inline void SetNormalY(Vec3& v) { v.y = 1.0f; }

// NONMATCHING: RALLYTROPHY 0x46c2c0
// 2026-11-16 round 12: SetNormalY moved to every later statement position (2-3 insns worse), helpers for x/z, y+touching/active, flat/volatile y at every position: none beat 1 insn. pop edi always lands right after the y store (original: after z, before y).
// 2026-11-16: normal.y via inline SetNormalY (no volatile) 27 bytes/2 insns -> 8 bytes/1 insn; only the y store still lands before pop edi (original after). Vec3 ctor (init list 11 insns, body temp 15), flat x/y/z permutations, helper/reference/pointer forms all 2+ insns.
// 2026-11-12: the early ecx reload is the epilogue's fs:0 restore load; the original schedules it before the x/z stores and the y=1 store after pop edi. Whole-vector forms (inline Set helper, Vec3(0,1,0) temporary or named copy, static const, x/y/z orders) are 2 insns or worse (temporaries add a 16-byte stack copy).
// Same size (317). Making only normal.y volatile reduces the mismatch from 66 to 27 bytes, but the target reloads ecx
// before the three stores and pops edi between z and y; this build reloads ecx and pops edi afterward. Volatile x or z
// individually, with y volatile, leaves the 27-byte diff unchanged. Plain objcmp.
// Tried 2026-10-30 night: volatile on x/z/y/touching/active in 14 combos plus reorders of the normal stores; best stays 27 bytes (y volatile only), none moves the ecx reload or pop edi.
// Tried 2026-10-31 re-pass: ~20 more barrier variants (this-cast volatile on x/z/y, Wheel* volatile self, volatile touching/active, volatile dummy reads of x/touching, y moved after touching) all >= 2 insns/27 bytes or worse; permute.py -r 40 stayed at (2, 27); ecx reload + pop edi still land after the y store.
// 2026-11-24: objcmp isolates the remaining mismatch to pop edi vs normal.y store (original pops first); 150 additional C++ store-form/order variants did not improve the current 8-byte diff.
// 2026-11-24: two 100-round permute.py searches (seeds 4622272 and 4622293; 24 candidates/round) retained baseline score (1 differing instruction, 8 bytes). The second search used the REA-confirmed POP EDI / normal.y ordering; no candidate improved it.
// REA assembly Evidence ev_8f3aeb9a325f8deb28a2f24f17b61dbe20d36f1a2f70c456a0dce7247a555e19 confirms POP EDI at 0x46c3a8 before normal.y at 0x46c3a9.
// Inline-asm hypothesis rejected: inserting `__asm { pop edi }` here made VC6 retain its own later POP EDI, switch to a different frame, and emit 321 bytes (220 differing); removed, returning to 317 bytes / 8 differing.
// VC6 label-scheduling probe (MCM2 VC6 SP3 notes: https://andrewboudreau.github.io/motocross-madness-2-decomp/VC6_OPERAND_ORDER.html): an unused label between z and y emitted no bytes but also did not move POP EDI; 317 bytes / 8 differing.
// LoadScreen::Init is an exact volatile-store precedent, but `normal.z` volatile here moved the FS-chain reload below x/z (24 differing bytes); with direct `normal.y`, it also sank y after POP ESI (71 bytes). Reverted both forms.
// REA Evidence ev_9efca54de0d92e333777e9c90f3f70444c23aeede431b7ae610b21559d6167d0: LoadScreen::Init at 0x40370e pops EDI after colorA[3] and before wrapV; its EAX=1.0f remains live for the later colorB stores.
// Setter/lifetime follow-up: float&, float*, Wheel*, and Vec3& aliases placed before Reset, after Reset, or after z all emitted POP EDI before x/z and y after POP ESI (66 differing bytes); reverted.
// REA Evidence ev_b5a27c938f9d4e57571e7729afa813fbfafd7a40d3656f2983d8bb45a0ef405d re-reads the full 317-byte body: EDI's visible uses end in the conditional string-copy path at 0x46c355, but its restore is at 0x46c3a8 after Reset and the normal.x/z stores. Observation: there is no EDI use between that path and POP; inference: the late restore is constrained by VC6's function/control-flow lowering rather than by the normal.y value itself. This narrows follow-up probes to lifetime/scope or EH/control-flow shaping; it does not prove which source construct caused the original placement.
// Scope-exit finalizer probe (MCM2 VC6_SHRINK_WRAP.md suggests scope/lifetime can affect saved-register placement): whether its lifetime began before z or after x/z, VC6 moved POP EDI before both x/z stores and emitted y after POP ESI (66 differing bytes); reverted.
// Chained-zero probe (`normal.z = normal.x = 0`) preserved the x-then-z stores but emitted the same 317-byte/8-byte mismatch as separate assignments; reverted.
// Identical-arm CFG probe (`if (normal.x == 0) y=1; else y=1`) was folded to the baseline 317-byte body/8-byte mismatch; reverted.
// NameString::Assign probe (the inline header body matches the manual operation semantically) changed the frame by 8 bytes and left 250 differing bytes; reverted to retain the exact early string-copy sequence.
// Returning `Vec3&` from SetNormalY with the call result discarded reproduced the setter-family regression: POP EDI before x/z and y after POP ESI (66 differing bytes); reverted.
// Empty Vec3 EH-fence scope around x/z emitted the baseline bytes unchanged; VC6 eliminated the unused local/lifetime boundary, so it did not create a restore point; reverted.
// Calling an inline Vec3 member setter with implicit `this` emitted the same 317-byte body/8-byte POP order as SetNormalY(Vec3&); reverted the temporary header method.
// do-while exit-condition probe (x/z in body, y store in the always-false condition) moved POP EDI before x/z and y after POP ESI (66 differing bytes); reverted.
// Local `float* normalZ = &normal.z; *normalZ = 0;` optimized to the same 317-byte/8-byte diff, so it supplied no new last-use boundary; reverted.
// VC6 compile-order probe: moving the matching Reset() body before Wheel::Wheel (the documented "already compiled means non-throwing" case) left the ctor at 317 bytes/8 differing; reverted.
// Local integer-bit constant inside SetNormalY folded to the same store and retained the baseline 317-byte/8-byte order; reverted.
// Inline float& accessor (`NormalY(normal) = 1.0f`) also optimized to the baseline store/pop order (317 bytes/8 diff); reverted.
// REA Evidence ev_6a1c66644636e5b321fef25c8a9241a27a458c048117d426f2a17edfced8a6a8 identifies the active target by SHA-256 and confirms 80 instructions/3 blocks; ev_8f3aeb9a325f8deb28a2f24f17b61dbe20d36f1a2f70c456a0dce7247a555e19 records the final sequence: stores to +0x2b4/+0x2bc, POP EDI, then immediate 1.0f at +0x2b8.
// Same-EXE search ev_3c14197d7868ba032d0fa0b5ec40abbe68dbebf4480ad8f6e0bc234782261c46 found that exact POP-EDI/1.0f store pattern only here. The related Grow/memcpy constructor at 0x4b7a40 restores EDI before field stores (REA ev_f3fc72493afd171f022c655312c26d8316e265b94ed415839c3c48a3cfdc83f6), but its 1.0f store is after POP ESI; its source form does not explain Wheel's split.
// Isolated cpp_permuter probes, all with objcmp score (base 8 bytes): dead_wrap had no applicable loop; scope_block compiled 2 candidates with no gain; inline_local and invert_if generated none. A manual flat-y + inline-tail-helper candidate compiled to 317 bytes but scored 66 differences, moving y after POP ESI; reverted. No candidate was copied into the main function.
// Further controls: DrawContext::DrawContext at 0x4b7a40 is byte-exact at 393 bytes by objcmp, but its explicit string pointer/compound length-index form did not transfer. In Wheel, pointer/reference/direct-nameStorage access and `d[nm.len = n] = 0` each retained the 8-byte diff. Grouping normal.x/z and the post-y tail into inline helpers around a flat y store produced 66 differing bytes and moved POP EDI before both x/z; reverted.
// Isolated exhaustive cpp_permuter search: reuse_local/cache_member/sink_decl/hoist_decl generated 3 candidates with no improvement; temp_for_expr/cond_temp generated 9 (2 failed to compile), also no improvement over score 8.
// 2026-11-24: a destructorless local wrapper (`WheelNormalYInit setNormalY(normal)`) compiled to the same 317-byte/8-difference baseline; reverted.
// New REA evidence: ev_244a11d494ca2f94ca154e02e08716b56cd7f33bfa28a2ce758024631937b5e4 decodes 0x46c3a8 as the one-byte POP EDI (5f); ev_994ee11bd78711aaafb8e985937356141762d90bb1393a83a8a01efd2902a30b confirms the 317-byte body; ev_9f7bb327215f2921bbcb5bdd6ef2be055ff2196987b4885d74a8ff46643d3105 records three blocks, with the final block from 0x46c361 through 0x46c3fc containing Reset, x/z, POP EDI, y and the remaining stores.
// Targeted source-boundary probes: `normal.y = (normal.z = 0, 1.0f)` and `touching = (normal.y = 1.0f, 0)` each built 317 bytes but scored 66 differing bytes / 2 instructions; a local scope guard whose inline destructor writes normal.y scored the same. Reverted. MCM2's VC6 SP3 BikeAI near-miss documents x/z/y store order and goto-tail forms that did not change CFG layout; this is a compiler-version lead only, not a transferable VC6 SP5 fix: https://andrewboudreau.github.io/motocross-madness-2-decomp/NEAR_MISS_INDEX.html
// Scalar helper control: `SetNormalYRef(normal.y)` and `SetNormalYPtr(&normal.y)` each scored 66 differing bytes / 2 instructions. A Grow-success early return with a duplicated, semantically equivalent constructor tail (motivated by MCM2's VC6 SP3 shrink-wrap notes) built 328 bytes and scored 226 differing bytes / 25 instructions; reverted. https://andrewboudreau.github.io/motocross-madness-2-decomp/VC6_SHRINK_WRAP.html
// `__forceinline` on an otherwise identical SetNormalY helper produced the same 317-byte / 8-byte-difference result as `inline`; the expansion hint does not alter this store/pop schedule. Reverted.
// VC6 `/FAs` listing on an isolated best-source compile maps the normal-field tail to source lines 252-264 as one range; candidate code emits y at offset 0xe8 and compiler-generated POP EDI at 0xf2. The listing gives no finer source-line boundary for that restore; objcmp remains 317 bytes / 8 differing, so this is diagnostic evidence only.
// Volatile `forceA.z` as a post-y memory-order barrier built 317 bytes but scored 43 differing bytes / 2 instructions; objcmp shows the saved-exception-list reload and POP EDI both moved after normal.y, touching/active and forceA.z. Reverted.
// Two-argument `__forceinline` scalar setters with `(value, y)` and `(y, value)` parameter order each built 317 bytes but scored 66 differing bytes / 2 instructions; reverted.
// 2026-11-24 follow-up: REA byte search ev_d5f9ac3d5de0447a96d5782bbf843f838d5cb97bc0329f5b29243eb01db613b1 found POP EDI + immediate field-store only at this constructor. Same-EXE examples: AIDriver::Reset 0x49ba70 restores EDI before its final field-init group (ev_05450131334e08e337dda211e01d2956011ab1e65172bfc0d4bd20ef7f58d69c); Sky::Sky 0x4758f0 restores EDI between global-zero stores and `scale` (ev_6219df4a63b9bb02afa3ef975f28988af8982455d34e33fe9083104d2a2403f9), and its source passes the focused matcher (13 marked functions match, 1 NONMATCHING in the file). The same-class LoadModel string paths show branch-local POP EDI positions (ev_4b83bbcdd62337238d0d52abcfb70a2261303a3d7dfd455bcc8733c1921869ca); these establish that VC6 can place the restore between stores, but do not identify a transferable source boundary for Wheel's straight-line tail.
// External VC6 SP3 note (CMR2Decomp rename_search.py) says local names can break equal-use stack-slot ties. Isolated VC6 SP5 renames `n->n2`, `d->text`, and `nm->nameString` each kept 317 bytes / 8 differing; this mismatch is not a stack-slot tie. Empty `__asm {}` inserted before y built 320 bytes / 227 differing; `_ReadWriteBarrier()` with an old-style declaration and `#pragma intrinsic` was emitted as a call, building 322 bytes / 98 differing. Both candidates were isolated and discarded.
// 2026-11-24: extended that rename test to the parameter of the only inline helper used in the ctor (`SetNormalY(Vec3& v)` -> `Vec3& normalY`); isolated unit build stayed 317 bytes / 8 differing, with the same POP EDI / y-store order.
// Fresh REA instruction Evidence ev_89a3b02d2da89783dc9654ecfb59efcbc1c88081256430a087769bb0ef18836b decodes the +0x2bc zero store at 0x46c3a2; ev_6b701ecb728f2f0ae78826cfcec3ecd55da17c4c2d59b6d6164f1a8cb757e4bd decodes the following +0x2b8 immediate store at 0x46c3a9. Together with the existing 0x46c3a8 instruction observation, this confirms the original order in the same EXE SHA-256.
// Source-shape follow-up: templating SetNormalY, adding a const local inside it, grouping all post-y stores in an inline Wheel& helper, representing 1.0f via its integer bits, and locally enabling optimize("s") each left the 317-byte baseline unchanged except the bit-store (66 bytes / 2 insns worse). No candidate improved the remaining 8-byte / 1-insn schedule; all stayed isolated.
Wheel::Wheel()
{
    tire.SetParent(this);
    tire.flags = 0x7f;
    {
        unsigned int n = strlen("tire null obj");
        NameString& nm = Name();
        if (nm.Grow(n, true)) {
            char* d = nm.ptr;
            memcpy(d, "tire null obj", n);
            nm.len = n;
            d[n] = 0;
        }
    }
    unk8 = 0;
    unk31c = unk318 = 0.0004f;
    unk320 = 1.5f;
    unk324 = 2.0f;
    unk328 = 0;
    Reset();
    normal.x = 0;
    normal.z = 0;
    SetNormalY(normal);
    touching = 0;
    active = 0;
    forceA.z = 0;
    forceA.y = 0;
    forceA.x = 0;
    surface = 0;
    unk2a0 = 0;
    steer = 0;
    radius = 0.3f;
}

// FUNCTION: RALLYTROPHY 0x46c400
// SYMBOL: ??_GWheel@@UAEPAXI@Z
typedef void Generated_46c400();

// FUNCTION: RALLYTROPHY 0x46c420
Wheel::~Wheel()
{
}

// FUNCTION: RALLYTROPHY 0x46c480
void Wheel::LoadModel(const char* name)
{
    tire.meshes.Load(name, 0);
    tire.vf2();
    tire.Name() = "tire mesh";
}

static inline float WrapAngle(float x) { return (float)fmod(x, 6.2831854820251465); }

// FUNCTION: RALLYTROPHY 0x46c550
// The volatile pre-fmod store and nested result assignment preserve the target store order; see
// docs/analysis/wheel-spin-matrix.md.
void Wheel::UpdateSpinMatrix(float dt)
{
    *(volatile float*)&angle = dt * spin + angle;
    float a;
    angle = (a = WrapAngle(angle));
    RawMatrix steerMatrix;
    RawMatrix spinMatrix;
    steerMatrix.SetIdentity();
    steerMatrix.FromAxisAngle(Vec3(0, 0, 1.0f), steer * 0.0872664675116539f);
    spinMatrix.FromEuler(a, 0, 0);
    tire.matrix.Multiply(&steerMatrix, &spinMatrix);
}

// FUNCTION: RALLYTROPHY 0x46c6b0
// original: FUN_0046c6b0
// Evidence: ev_0c15dff8d085af1e417247cb7b4733d3841de445d06b0ae4852cb5506bd89e1e
// Pin the first three stores before the shared 1.0f constant load.
void Wheel::Reset()
{
    *(volatile int*)&forceA.z = 0;
    *(volatile int*)&forceA.y = 0;
    *(volatile int*)&forceA.x = 0;
    torqueB.z = 0;
    torqueB.y = 0;
    torqueB.x = 0;
    unk27c = 0;
    extraTorque = 0;
    spin = 0;
    slipA = 0;
    slipB = 0;
    load = 0;
    angle = 0;
    active = 0;
    unk2e4 = 0;
    params.z = 0;
    share = 1.0f;
    normal.x = 0;
    normal.y = 1.0f;
    normal.z = 0;
    surface = 0;
    unk30c = 0;
    unk310 = 0;
    unk314 = 0;
    unk2f4 = 0;
    unk2f8 = 0;
    unk2f0 = 0;
    value = 0;
    unk2fc = 0;
    unk304 = 0;
    unk308 = 0;
    params.x = 0.005f;
    params.y = 2.0f;
}

static inline void WheelAddTorque(float* t, const Vec3& v) { float x = t[0]; t[0] = x + v.x; t[1] = v.y + t[1]; t[2] = v.z + t[2]; }
// NONMATCHING: RALLYTROPHY 0x46c790
// original: FUN_0046c790
// Pass 2026-11-16 (L1): lon sqrt argument written as y*y+z*z+lx2 (not CSE'd with lonLen2, which is z*z+y*y then +lx2; same sums, commuted operands); 388->385 insns differ, 3501->3509 bytes. Tried: lat sqrt recompute (insns 387-391, bytes 3525), g4 reassociated sum (rejected, changes order).
// Pass 2026-11-13: ld assigned inside the && condition, torque add via inline WheelAddTorque(FUN_004203e0(...)) (uses the returned eax), arm built as a Vec3 argument temp; 412->388 insns differ, 3511->3501 bytes.
// Re-pass 2026-11-01: float-sum audit vs original fadd order; fixed latVel/lonVel (x,z,y chain), latLen2 (y,x,z chain), blendSpeed and slip relax (K before unk2ec); all other sums and field reads already agree; 420->414 insns differ, rest is frame size 0xfc vs 0xe4 and x87 lifetimes.
// Tried 2026-10-30 night: ~16 variants (load int local 433->420, vel field-wise, Vec3/array localPos, decl order, volatile S/ld, slip copy, fy/L2 split); remaining diff is frame size 0xfc vs 0xe4 (asm pointer temps share float slots in the original), missing EH-state store timing, and x87 lifetimes.
// 3506 vs 3573 bytes (2843 differing bytes). Stiction vector declaration groups reproduce target EH transitions 8,
// 0xb and 0xd before the scale, subtraction, and output stages (REA Evidence
// ev_e435fac9d046ad9215a458d11ef7434b9660e6fc42ca1f6de1e8904630099a6d). Remaining differences include stack slots
// (target asm pointer temporaries share slots with float locals), x87 lifetimes, and store scheduling.
// Tyre forces of one wheel: contact patch velocity, relaxed slip ratio/angle, Pacejka lateral and longitudinal
// force with a friction-circle limit, low-speed stiction, and the reaction applied to the car body.
void Wheel::UpdateForces()
{
    unk268 = unk264;
    WheelBody* body = (WheelBody*)((Car*)car)->body;
    value = 0.0f;
    unk2fc = 0.0f;
    unk304 = 0.0f;
    unk308 = 0.0f;
    int ld;
    if (active != 0 && (ld = *(const int*)&load) > *(const int*)&kMinLoad) {
        Vec3 lat, lon, contact, vel;

        // Contact point and its velocity in world space.
        Vec3 localPos(*(Vec3*)position);
        const float* matrix = body->rotation;
        const float* trans = body->position;
        const float* point = (const float*)&localPos;
        float* result = (float*)&contact;
        WHEEL_XFORM_ADD();
        vel = *(Vec3*)body->velocity;
        vel += body->FUN_004204f0(contact);

        // Wheel axle and forward axis in world space.
        Vec3 axleW, forwardW;
        matrix = body->rotation;
        point = axle;
        result = (float*)&axleW;
        WHEEL_XFORM();
        point = forward;
        result = (float*)&forwardW;
        WHEEL_XFORM();

        // Ground-plane directions: longitudinal = axle x normal, lateral = normal x forward.
        lon = FUN_004203e0(axleW, normal);
        float lx2 = lon.x * lon.x; float lonLen2; lonLen2 = lon.y * lon.y; lonLen2 = lon.z * lon.z + lonLen2; lonLen2 = lonLen2 + lx2;
        if (lonLen2 < 0.0001f) {
            lon = forwardW;
        } else {
            float inv = 1.0f / (float)sqrt(lon.y * lon.y + lon.z * lon.z + lx2);
            lon.x = inv * lon.x;
            lon.y = inv * lon.y;
            lon.z = inv * lon.z;
        }
        lat = FUN_004203e0(normal, forwardW);
        float latLen2; latLen2 = lat.y * lat.y; latLen2 = lat.x * lat.x + latLen2; latLen2 = latLen2 + lat.z * lat.z;
        if (latLen2 < 0.0001f) {
            lat = *(Vec3*)up * *(const RotMatrix*)body->rotation;
        } else {
            float inv = 1.0f / (float)sqrt(latLen2);
            lat.x = inv * lat.x;
            lat.y = inv * lat.y;
            lat.z = inv * lat.z;
        }

        // Camber sine (signed) from the axle against the ground lateral direction.
        float cq = axleW.z * lat.z; cq = axleW.y * lat.y + cq; cq = axleW.x * lat.x + cq;
        float c = (float)fabs(cq);
        if (!(c < 1.0f)) {
            c = 1.0f;
        }
        float camber = (float)sqrt(1.0f - c * c);
        if (axleW.z * normal.z + axleW.y * normal.y + axleW.x * normal.x > 0.0f) {
            camber = -camber;
        }

        // Load dependent tyre coefficients from the surface block.
        TyreSurface* S = surface;
        float L = load * 0.001f;
        float L2 = L * L;
        float stiffLat = L2 * S->p10 + L * S->p14;
        float peakLat = (float)sin(2.0 * atan(L / S->loadRef)) * S->peakScale;
        float stiffLon = L2 * S->p28 + L * S->p2c;
        float peakLon = L * S->p34 + S->p30;
        float peakLat2 = L * S->p3c + S->p38;
        params.z = (peakLat2 + peakLon) * load * 0.5f;
        float ratio = peakLon * peakLat / (peakLat2 * stiffLat);
        float invLoad = 1.0f / (load * peakLat2);

        // Contact velocity along the ground directions.
        float latVel; latVel = vel.x * lat.x; latVel = vel.z * lat.z + latVel; latVel = vel.y * lat.y + latVel;
        float lonVel; lonVel = vel.x * lon.x; lonVel = vel.z * lon.z + lonVel; lonVel = vel.y * lon.y + lonVel;
        slipB = latVel;
        unk308 = latVel;
        float wheelSpeed = spin * unk268;
        float slipSpeed0 = wheelSpeed - lonVel;
        slipA = slipSpeed0;

        // Slip ratio: direct at speed, relaxed (first order) below the stiction speed.
        float blendSpeed; blendSpeed = (float)fabs(lonVel) * DAT_0050941c; blendSpeed = blendSpeed * unk2ec; blendSpeed = blendSpeed * 2.0f;
        float slipRatio;
        if (!(blendSpeed < 1.0f)) {
            slipRatio = slipSpeed0 / (float)fabs(lonVel);
            unk2f8 = -slipRatio;
        } else {
            if (((*(int*)&unk2f0 ^ *(int*)&lonVel) & 0x80000000) != 0) {
                unk2f8 = unk2f8 * -0.7f;
            }
            float dd = ((lonVel - wheelSpeed) - (float)fabs(lonVel) * unk2f8); dd = dd * DAT_0050941c; dd = dd * unk2ec; unk2f8 = dd + unk2f8;
            if (unk2f8 > 10.0f) {
                unk2f8 = 10.0f;
            } else if (unk2f8 < -10.0f) {
                unk2f8 = -10.0f;
            }
            slipRatio = -unk2f8;
        }
        value = slipRatio;
        unk2f0 = lonVel;
        unk304 = slipSpeed0;
        float slipRatioN = slipRatio * stiffLat / (load * peakLon);

        // Lateral slip tangent, relaxed in the same way.
        unk2f4 = (latVel - unk2f4 * (float)fabs(lonVel)) * unk2e8 * DAT_0050941c + unk2f4;
        if (blendSpeed < 1.0f) {
            unk2f4 = (1.0f - blendSpeed) * (float)(latVel / (fabs(lonVel) + 0.0099999997764825821)) + unk2f4 * blendSpeed; // the original's double is (double)0.01f
        }
        float slipTan = unk2f4;
        float alpha = (float)atan(slipTan);
        unk2fc = alpha;
        float latSlipN = slipTan * invLoad * peakLat;

        float t = camber * (invLoad * stiffLon);
        if (alpha >= 0.0f) {
            t = 1.0f - t;
        } else {
            t = t + 1.0f;
        }
        float latSlip;
        if (t < 0.0001f) {
            latSlip = latSlipN * 10000.0f;
        } else {
            latSlip = latSlipN / t;
        }

        float fy = Pacejka(latSlipN * S->lonPeakScale, 0.852f, 2.3f, 0.51f, -2.75f);
        if (side >= 2) {
            fy = -fy;
        }
        fy = load * S->latScale * fy * peakLat2;

        // Combined slip, friction circle.
        float comb = (float)sqrt(latSlip * latSlip + slipRatioN * slipRatioN);
        unk294 = comb;
        float blend;
        if (comb > 6.2831855f) {
            blend = 1.0f;
        } else {
            blend = (ratio + 1.0f) * 0.5f - (0.5f - ratio * 0.5f) * (float)cos(comb * 0.5f);
        }
        float g = Pacejka(comb, 0.714f, 1.4f, 1.0f, -0.2f);
        float h = (float)sqrt(blend * blend * slipTan * slipTan + slipRatio * slipRatio);
        if (h < 0.001f) {
            h = 0.001f;
        }
        float lonN = g * slipRatio / h;
        float rest = g * g - lonN * lonN;
        float latN;
        if ((*(int*)&rest & 0x80000000) != 0) {
            latN = 0.0f;
        } else {
            latN = (float)sqrt(rest);
        }
        if (!(alpha < 0.0f)) {
            latN = -latN;
        }
        float lonForce = load * lonN * peakLon;
        float latForce = load * latN * peakLat2 + unk26c * fy;

        // Weakens the rolling resistance terms for large slip angles.
        float limit = params.x * 1.5707964f;
        if ((float)fabs(alpha) < limit) {
            unk310 = 1.0f;
        } else {
            float u = (float)fabs(latVel) * (((float)fabs(alpha) - limit) / (1.5707964f - limit)) / params.y;
            unk310 = 1.0f - u * u;
        }
        unk310 = load * unk31c * unk310 * peakLat2;
        unk314 = unk328 * unk318 * load * peakLon;
        torqueB.z = (float)fabs((float)sin(alpha) * latForce) * S->alignScale;
        if ((*(int*)&spin & 0x80000000) != 0) {
            torqueB.z = -torqueB.z;
        }
        torqueB.x = 0.0f;
        torqueB.y = 0.0f;

        // Static friction at very low speed: pull the sliding velocity to zero, limited by the car's inertia.
        if (blendSpeed < 1.0f) {
            bool latStick = (float)fabs(latVel * DAT_00509418) < peakLat2 * 9.81f;
            float limLon = peakLon * 9.81f;
            bool lonStick = (float)fabs(slipSpeed0 * DAT_00509418) < limLon;
            if (latStick || lonStick) {
                Vec3 slip, dir;
                slip = latVel * lat;
                slip += slipSpeed0 * lon;
                dir = slip;
                float lq = slip.z * slip.z; lq = slip.y * slip.y + lq; lq = slip.x * slip.x + lq;
                float len = (float)sqrt(lq);
                if (len > 0.00001f) {
                    float inv = 1.0f / len;
                    dir.x = slip.x * inv;
                    dir.y = slip.y * inv;
                    dir.z = slip.z * inv;
                }
                Vec3 r, axis, tmp;
                r = contact - *(Vec3*)body->centre;
                axis = FUN_004203e0(r, dir);
                const float* matrix2 = body->inertia;
                const float* point2 = (const float*)&axis;
                float* result2 = (float*)&tmp;
                {
                    const float* point = point2;
                    const float* matrix = matrix2;
                    float* result = result2;
                    WHEEL_XFORM_ROW();
                }
                {
                    const float* a = (const float*)&tmp;
                    const float* b = (const float*)&r;
                    float* result = (float*)&axis;
                    WHEEL_CROSS();
                }
                float effInv = body->invMass + (axis.z * dir.z + axis.y * dir.y + axis.x * dir.x);
                float k = load * len / (body->mass * effInv * 9.81f);
                if (latStick) {
                    latForce = latForce * blendSpeed - (dir.z * lat.z + dir.y * lat.y + dir.x * lat.x) * (1.0f - blendSpeed) * k * DAT_00509418;
                }
                if (lonStick) {
                    float r2 = (float)fabs(slipSpeed0 * DAT_00509418) / limLon;
                    float y = (dir.z * lon.z + dir.y * lon.y + dir.x * lon.x) * k * DAT_00509418;
                    lonForce = r2 * lonForce + (1.0f - r2) * y;
                }
            }
        }

        // Output force in wheel axes and its reaction on the car body.
        forceA.x = latForce;
        forceA.y = 0.0f;
        forceA.z = lonForce;
        Vec3 F;
        F.x = lat.x * latForce + lon.x * lonForce;
        F.y = lat.y * latForce + lon.y * lonForce;
        F.z = lat.z * latForce + lon.z * lonForce;
        body->force[0] = F.x + body->force[0];
        body->force[1] = F.y + body->force[1];
        body->force[2] = F.z + body->force[2];
        WheelAddTorque(body->torque, FUN_004203e0(F, Vec3(contact.x - body->centre[0], contact.y - body->centre[1], contact.z - body->centre[2])));
        unk2e4 = load * S->grip;
    } else {
        unk2f4 = unk2f8 = unk314 = unk310 = unk30c = params.z = torqueB.x = torqueB.y = torqueB.z = unk2e4 = forceA.x = forceA.y = forceA.z = 0.0f;
    }
    torqueB.z = (steer * 0.98100006580352783f + 0.0098100006580352783f) * (spin * unk278 * unk264) + torqueB.z; // the original's floats are 1 ulp above 0.981f / 0.00981f
}

// FUNCTION: RALLYTROPHY 0x46d590
// original: FUN_0046d590
static float Pacejka(float x, float B, float C, float D, float E)
{
    float bx = x * B;
    float y = (float)(E * atan(bx) + (1.0f - E) * bx);
    return (float)(sin(C * atan(y)) * D);
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// original: FUN_0046d5d0
__declspec(naked)
// FUNCTION: RALLYTROPHY 0x46d5d0
void Wheel::IntegrateSpin(float dt)
{
    __asm {
        sub esp, 0xc
        push esi
        mov esi, ecx
        fld dword ptr [esi + 0x2e0]
        mov eax, dword ptr [esi + 0x280]
        fadd dword ptr [esi + 0x2d4]
        mov ecx, dword ptr [esi + 0x270]
        mov dword ptr [esp + 0xc], eax
        mov eax, dword ptr [esi + 0x29c]
        test eax, eax
        fmul dword ptr [esi + 0x268]
        mov dword ptr [esp + 4], ecx
        fchs
        fstp dword ptr [esp + 8]
        fld dword ptr [DAT_004e134c]
        je wheel_spin_no_drive
        mov eax, dword ptr [esi + 0x130]
        fstp st(0)
        push edi
        lea edi, [eax + 0xdb8]
        mov ecx, edi
        call fun_0046aff0
        fmul dword ptr [esi + 0x32c]
        mov ecx, edi
        fmul dword ptr [DAT_004e138c]
        fadd dword ptr [esp + 8]
        fstp dword ptr [esp + 8]
        call fun_0046afb0
        fmul dword ptr [esi + 0x32c]
        pop edi
        fmul dword ptr [DAT_004e138c]
    wheel_spin_no_drive:
        fld dword ptr [DAT_004e1348]
        fdiv dword ptr [esp + 4]
        fstp dword ptr [esp + 4]
        fadd dword ptr [esp + 8]
        fmul dword ptr [esp + 4]
        fmul dword ptr [esp + 0x14]
        fadd dword ptr [esi + 0x284]
        fst dword ptr [esi + 0x284]
        fld dword ptr [esi + 0x2e4]
        fmul dword ptr [esi + 0x268]
        fadd dword ptr [esp + 0xc]
        fmul dword ptr [esp + 4]
        fmul dword ptr [esp + 0x14]
        fld st(0)
        fabs
        fld st(2)
        fabs
        fxch st(1)
        fxch st(1)
        fcompp
        fnstsw ax
        test ah, 1
        je wheel_spin_clamp
        fstp st(0)
        mov dword ptr [esi + 0x284], 0
        fstp st(0)
        pop esi
        add esp, 0xc
        ret 4
    wheel_spin_clamp:
        mov eax, dword ptr [esi + 0x284]
        fxch st(1)
        test eax, 0x80000000
        je wheel_spin_nonnegative
        fadd st, st(1)
        fstp dword ptr [esi + 0x284]
        pop esi
        fstp st(0)
        add esp, 0xc
        ret 4
    wheel_spin_nonnegative:
        fsub st, st(1)
        fstp dword ptr [esi + 0x284]
        pop esi
        fstp st(0)
        add esp, 0xc
        ret 4
    }
}

// Module end: guard-bit initialiser of the STL header statics and its atexit registration.

// FUNCTION: RALLYTROPHY 0x46d6f0
// SYMBOL: _$E23
typedef void Generated_46d6f0();

// FUNCTION: RALLYTROPHY 0x46d710
// SYMBOL: _$E22
typedef void Generated_46d710();
