#ifndef UNK_4B5140_H
#define UNK_4B5140_H
#include "unk_41b9e0.h"
// The network header uses CarBody for a separate partial class view.
#define CarBody NetworkCarBody
#include "unk_41c390.h"
#undef CarBody
#include "unk_47c460.h"
#include "unk_46c2c0.h"
#include "unk_4b9d30.h"
// Additional matrix entry points not declared on the existing matrix aliases.
struct BasisMatrix {
    float m[16];
    BasisMatrix(const FV3& a, const FV3& b, const FV3& c);
    BasisMatrix* SetBasis(const FV3& a, const FV3& b, const FV3& c); // constructor entry alias, 0x4b5430

};
#endif
