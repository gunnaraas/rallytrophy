#include "rallytrophy.h"
#include "scene_object.h"
#include <math.h>

// Original vector at 0x57b960: begin +4, end +8, capacity end +0xc.
static std::vector<ScenePoint*> s_spherePoints;

// The compiler emits this four-function initializer/destructor group for the
// file-static vector above.
// FUNCTION: RALLYTROPHY 0x4b44e0
// SYMBOL: _$E18
typedef void Generated_4b44e0();
// FUNCTION: RALLYTROPHY 0x4b4500
// SYMBOL: _$E20
typedef void Generated_4b4500();
// FUNCTION: RALLYTROPHY 0x4b4510
// SYMBOL: _$E19
typedef void Generated_4b4510();

// FUNCTION: RALLYTROPHY 0x4b4530
// original: FUN_004b4530
// Evidence: ev_44ce6879226232bef8d09d13cf61fca678c7b7fff2086db9282a1d70f6dcbe48
// The cursor is initialized only after the positive-count test; VC6 retains
// the observed stack-slot copy. Vertex records have the observed 44-byte stride.
void SceneSphere::AppendVertices(SceneVertex* vertices, int count, int build)
{
    if (count) {
        s_spherePoints.reserve(s_spherePoints.size() + count);
        if (count > 0) {
            SceneVertex* cursor = vertices;
            int remaining = count;
            do {
                s_spherePoints.push_back(&cursor->position);
                ++cursor;
            } while (--remaining);
        }
    }
    if (build) Build();
}

// FUNCTION: RALLYTROPHY 0x4b47e0
// original: FUN_004b47e0
void SceneSphere::ResetPoints()
{
    s_spherePoints.resize(0);
}

// FUNCTION: RALLYTROPHY 0x4b4820
// original: FUN_004b4820
void SceneSphere::Build()
{
    int count = s_spherePoints.size();
    if (count > 1) {
        BuildFromPoints(count);
    } else if (count) {
        SetPoint(s_spherePoints[0]);
    } else {
        center.x = center.y = center.z = 0;
        radius = 0;
    }
}

static void __cdecl SphereBoundary1(SceneSphere*, int, ScenePoint**, const ScenePoint*);
static void __cdecl SphereBoundary2(SceneSphere*, int, ScenePoint**, const ScenePoint*, const ScenePoint*);
static void __cdecl SphereBoundary3(SceneSphere*, int, ScenePoint**, const ScenePoint*, const ScenePoint*, const ScenePoint*);

int DAT_0057b970;
static const float kInitialSquaredRadius = 0.0f; // original bits at 0x4e134c

// FUNCTION: RALLYTROPHY 0x4b4870
// original: FUN_004b4870
// Randomized support-point sphere, with the original five-attempt fallback.
void SceneSphere::BuildFromPoints(int count)
{
    ScenePoint** points = s_spherePoints.begin();
    const ScenePoint* first = points[0];
    ScenePoint minimum(first->x, first->y, first->z);
    ScenePoint maximum(first->x, first->y, first->z);
    for (int i = 0; i < count; ++i) {
        const ScenePoint& p = *points[i];
        if (p.x < minimum.x) minimum.x = p.x;
        else if (p.x > maximum.x) maximum.x = p.x;
        if (p.y < minimum.y) minimum.y = p.y;
        else if (p.y > maximum.y) maximum.y = p.y;
        if (p.z < minimum.z) minimum.z = p.z;
        else if (p.z > maximum.z) maximum.z = p.z;
    }
    ScenePoint midpoint = (minimum + maximum) * 0.5f;
    float farthest = kInitialSquaredRadius;
    for (int j = 0; j < count; ++j) {
        ScenePoint delta = *points[j];
        delta -= midpoint;
        float squared = delta.SquaredLength();
        if (squared > farthest) farthest = squared;
    }
    float fallbackRadius = (float)sqrt(farthest);
    int attempts = 0;
    DAT_0057b970 = 1;
    do {
        points = s_spherePoints.begin();
        for (int k = count - 1; k > 0; --k) {
            int selected = rand() % (k + 1);
            if (selected != k) {
                ScenePoint* old = points[k];
                points[k] = points[selected];
                points[selected] = old;
            }
        }
        SetPoint(points[0]);
        for (int n = 1; n < count; ++n) {
            const ScenePoint* point = points[n];
            if (!Contains(point)) SphereBoundary1(this, n, points, point);
        }
        ++attempts;
        DAT_0057b970 = 1;
        // The original exits on an unordered comparison too.
        if (!(radius > fallbackRadius)) return;
    } while (attempts != 5);
    radius = fallbackRadius;
    center = midpoint;
}

// FUNCTION: RALLYTROPHY 0x4b4b00
// original: FUN_004b4b00
static void __cdecl SphereBoundary1(SceneSphere* sphere, int count, ScenePoint** points,
                                    const ScenePoint* a)
{
    sphere->SetPoint(a);
    for (int i = 0; i < count; ++i) {
        const ScenePoint* point = points[i];
        if (!sphere->Contains(point)) SphereBoundary2(sphere, i, points, point, a);
    }
}

// FUNCTION: RALLYTROPHY 0x4b4b60
// original: FUN_004b4b60
static void __cdecl SphereBoundary2(SceneSphere* sphere, int count, ScenePoint** points,
                                    const ScenePoint* a, const ScenePoint* b)
{
    sphere->SetPair(a, b);
    for (int i = 0; i < count; ++i) {
        const ScenePoint* point = points[i];
        if (!sphere->Contains(point)) SphereBoundary3(sphere, i, points, point, a, b);
    }
}

// FUNCTION: RALLYTROPHY 0x4b4bd0
// original: FUN_004b4bd0
static void __cdecl SphereBoundary3(SceneSphere* sphere, int count, ScenePoint** points,
                                    const ScenePoint* a, const ScenePoint* b, const ScenePoint* c)
{
    sphere->SetTriple(a, b, c);
    for (int i = 0; i < count; ++i) {
        const ScenePoint* point = points[i];
        if (!sphere->Contains(point)) sphere->SetQuad(point, a, b, c);
    }
}

// FUNCTION: RALLYTROPHY 0x4b4c40
// original: FUN_004b4c40
void SceneSphere::SetPoint(const ScenePoint* point)
{
    center = *point;
    radius = 0;
}

static const double kSphereHalf = 0.5; // original double at 0x4e2958

// FUNCTION: RALLYTROPHY 0x4b4c60
// original: FUN_004b4c60
void SceneSphere::SetPair(const ScenePoint* a, const ScenePoint* b)
{
    center.x = (a->x + b->x) * kSphereHalf;
    center.y = (b->y + a->y) * kSphereHalf;
    center.z = (a->z + b->z) * kSphereHalf;
    float x = b->x - a->x;
    float y = b->y - a->y;
    float z = b->z - a->z;
    float length = (float)sqrt(x * x + y * y + z * z);
    radius = length * kSphereHalf;
}

// FUNCTION: RALLYTROPHY 0x4b50c0
// original: FUN_004b50c0
int SceneSphere::Contains(const ScenePoint* point) const
{
    float x = point->x - center.x;
    float y = point->y - center.y;
    float z = point->z - center.z;
    if (x * x + y * y + z * z <= radius * radius) return 1;
    return 0;
}

static const double kSphereOne = 1.0;       // original: 0x4e2978
static const double kSphereDetEpsilon = 1.0e-6; // original: 0x4e29b0, bits 0x3eb0c6f7a0b5ed8d

// FUNCTION: RALLYTROPHY 0x4b4e80
// original: FUN_004b4e80
// Four-point circumsphere from a double-precision cofactor inverse.
void SceneSphere::SetQuad(const ScenePoint* a, const ScenePoint* b,
                          const ScenePoint* c, const ScenePoint* d)
{
    double ux = b->x - a->x;
    double uy = b->y - a->y;
    double uz = b->z - a->z;
    double vx = c->x - a->x;
    double vy = c->y - a->y;
    double vz = c->z - a->z;
    double offset[3];
    double rhs[3];
    double inverse[3][3];
    offset[0] = d->x - a->x;
    offset[1] = d->y - a->y;
    offset[2] = d->z - a->z;
    rhs[0] = (ux * ux + uy * uy + uz * uz) * kSphereHalf;
    rhs[1] = (vx * vx + vy * vy + vz * vz) * kSphereHalf;
    rhs[2] = (offset[0] * offset[0] + offset[1] * offset[1] + offset[2] * offset[2]) * kSphereHalf;
    inverse[0][0] = vy * offset[2] - vz * offset[1];
    inverse[0][1] = uz * offset[1] - uy * offset[2];
    inverse[0][2] = vz * uy - vy * uz;
    inverse[1][0] = vz * offset[0] - vx * offset[2];
    inverse[1][1] = ux * offset[2] - uz * offset[0];
    inverse[1][2] = vx * uz - vz * ux;
    inverse[2][0] = vx * offset[1] - vy * offset[0];
    inverse[2][1] = uy * offset[0] - ux * offset[1];
    inverse[2][2] = vy * ux - vx * uy;
    double determinant = ux * inverse[0][0] + uy * inverse[1][0] + uz * inverse[2][0];
    if (fabs(determinant) > kSphereDetEpsilon) {
        double scale = kSphereOne / determinant;
        for (int row = 0; row < 3; ++row) {
            for (int col = 0; col < 3; ++col) inverse[row][col] *= scale;
        }
        for (int i = 0; i < 3; ++i) {
            offset[i] = 0;
            for (int j = 0; j < 3; ++j) offset[i] += inverse[i][j] * rhs[j];
        }
        center.x = a->x + offset[0];
        center.y = a->y + offset[1];
        center.z = a->z + offset[2];
        radius = sqrt(offset[0] * offset[0] + offset[1] * offset[1] + offset[2] * offset[2]);
    }
}
