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

// NONMATCHING: RALLYTROPHY 0x46c2c0
// Same size (317); only the store schedule differs: the original issues `pop edi` after the normal.x/normal.z stores
// and the normal.y = 1.0 store right behind it, the build pops edi first and sinks the 1.0 store to the end.
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
    normal.y = 1.0f;
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

// NONMATCHING: RALLYTROPHY 0x46c550
// Same instruction stream except two dead-looking stores: the original stores the new angle to [this+0x290] and to
// the local `a` right after the add, before the fmod call; VC6 drops them here (all variants tried).
void Wheel::UpdateSpinMatrix(float dt)
{
    float a = dt * spin + angle;
    angle = a;
    angle = a = (float)fmod(a, 6.2831854820251465);
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

// NONMATCHING: RALLYTROPHY 0x46c790
// original: FUN_0046c790
// 3504 vs 3573 bytes; logic and call sequence complete (mnemonic sequence ~91% equal). Differences: stack slots (the
// original's asm pointer temporaries share slots with float locals), the original keeps lon/lat.z in the FPU after
// the cross-product call and caches x*x, the interleaving of the unk2f0/value/unk304 stores, and some fxch/fabs reloads.
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
    if (active != 0 && *(const int*)&load > *(const int*)&kMinLoad) {
        Vec3 lat, lon, localPos, contact, vel;

        // Contact point and its velocity in world space.
        const float* matrix = body->rotation;
        const float* trans = body->position;
        const float* point = (const float*)&localPos;
        float* result = (float*)&contact;
        localPos = *(Vec3*)position;
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
        if (lon.x * lon.x + lon.y * lon.y + lon.z * lon.z < 0.0001f) {
            lon = forwardW;
        } else {
            float inv = 1.0f / (float)sqrt(lon.y * lon.y + lon.z * lon.z + lon.x * lon.x);
            lon.x = inv * lon.x;
            lon.y = inv * lon.y;
            lon.z = inv * lon.z;
        }
        lat = FUN_004203e0(normal, forwardW);
        float latLen2 = lat.x * lat.x + lat.y * lat.y + lat.z * lat.z;
        if (latLen2 < 0.0001f) {
            lat = *(Vec3*)up * *(const RotMatrix*)body->rotation;
        } else {
            float inv = 1.0f / (float)sqrt(latLen2);
            lat.x = inv * lat.x;
            lat.y = inv * lat.y;
            lat.z = inv * lat.z;
        }

        // Camber sine (signed) from the axle against the ground lateral direction.
        float c = (float)fabs(axleW.z * lat.z + axleW.y * lat.y + axleW.x * lat.x);
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
        float latVel = vel.x * lat.x + vel.z * lat.z + vel.y * lat.y;
        float lonVel = vel.x * lon.x + vel.z * lon.z + vel.y * lon.y;
        slipB = latVel;
        unk308 = latVel;
        float wheelSpeed = spin * unk268;
        float slipSpeed0 = wheelSpeed - lonVel;
        slipA = slipSpeed0;

        // Slip ratio: direct at speed, relaxed (first order) below the stiction speed.
        float blendSpeed = (float)fabs(lonVel) * DAT_0050941c * unk2ec * 2.0f;
        float slipRatio;
        if (!(blendSpeed < 1.0f)) {
            slipRatio = slipSpeed0 / (float)fabs(lonVel);
            unk2f8 = -slipRatio;
        } else {
            if (((*(int*)&unk2f0 ^ *(int*)&lonVel) & 0x80000000) != 0) {
                unk2f8 = unk2f8 * -0.7f;
            }
            unk2f8 = ((lonVel - wheelSpeed) - (float)fabs(lonVel) * unk2f8) * DAT_0050941c * unk2ec + unk2f8;
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
            unk2f4 = (1.0f - blendSpeed) * (latVel / ((float)fabs(lonVel) + 0.01f)) + unk2f4 * blendSpeed;
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
                Vec3 slip, dir, r, axis, tmp;
                slip = latVel * lat;
                slip += slipSpeed0 * lon;
                dir = slip;
                float len = (float)sqrt(slip.z * slip.z + slip.y * slip.y + slip.x * slip.x);
                if (len > 0.00001f) {
                    float inv = 1.0f / len;
                    dir.x = slip.x * inv;
                    dir.y = slip.y * inv;
                    dir.z = slip.z * inv;
                }
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
        Vec3 arm;
        arm.x = contact.x - body->centre[0];
        arm.y = contact.y - body->centre[1];
        arm.z = contact.z - body->centre[2];
        Vec3 torque = FUN_004203e0(F, arm);
        body->torque[0] = body->torque[0] + torque.x;
        body->torque[1] = torque.y + body->torque[1];
        body->torque[2] = torque.z + body->torque[2];
        unk2e4 = load * S->grip;
    } else {
        unk2f4 = unk2f8 = unk314 = unk310 = unk30c = params.z = torqueB.x = torqueB.y = torqueB.z = unk2e4 = forceA.x = forceA.y = forceA.z = 0.0f;
    }
    torqueB.z = (steer * 0.981f + 0.00981f) * (spin * unk278 * unk264) + torqueB.z;
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
