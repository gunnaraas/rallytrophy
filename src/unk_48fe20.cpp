// Highscore tables (0x48fe20-0x49094f): class RecordTable, embedded in the game object at +0x5c30.
// Records are ScoreRecord (0x60 bytes); a list holds the ten best of one car class.
#include "unk_481390.h"
#include "unk_4843e0.h"
#include "rallytrophy.h"
#include <string.h>
#include <windows.h>

extern char DAT_005070b0[];            // default player name
extern int DAT_004ff344;               // file versions: 0x20010823
extern int DAT_004ff348;               // 0x20011129
extern int DAT_004ff34c;               // 0x20020111
extern int DAT_004ff350;               // 0x20020122 (current)
extern int DAT_004ff354[];             // car class remap for the older file versions
extern DWORD DAT_004ff3d4[];           // cipher key

// The file cipher (XXTEA over dwords); an empty class in the original (this is unused).
class Cipher {
public:
    Cipher() {}
    ~Cipher() {}
    void Encrypt(DWORD* data, DWORD* key, int count);   // 0x4010f0
    void Decrypt(DWORD* data, DWORD* key, int count);   // 0x401180
};

// FUNCTION: RALLYTROPHY 0x48fe20
RecordTable::RecordTable()
{
    dirty = 0;
    FUN_0048fe40();
}

// NONMATCHING: RALLYTROPHY 0x48fe40
// 382 vs 386 bytes. Same loops and stores; the original strength-reduces the loop pointer to &record->unk1f (edx) and
// keeps the record base in ebp for the name/time/unk5c accesses, this build uses the record itself.
void RecordTable::FUN_0048fe40()
{
    SYSTEMTIME now;
    GetLocalTime(&now);
    for (int g = 0; g < 5; g++) {
        int count = 11;
        int base;
        if (!g) {
            count = 13;
            base = 0;
        } else {
            base = g * 11 + 2;
        }
        for (int h = 0; h < count; h++) {
            for (int k = 0; k < 23; k++) {
                int car = k;
                if (!k) {
                    car = 1;
                }
                ScoreRecord* record = (ScoreRecord*)records + ((base + h) * 23 + k) * 10;
                for (int e = 0; e < 10; e++, record++) {
                    memset(record, 0, sizeof(ScoreRecord));
                    strcpy(record->name, DAT_005070b0);
                    record->date = now;
                    record->unk1e = 0;
                    record->unk1f = car;
                    int* split = record->splits;
                    for (int t = 0x8ca0; t < 0x60ae0; t += 0x8ca0) {
                        *split++ = t;
                    }
                    record->time = 0x57e40;
                    if (h < 3) {
                        record->time = 0x36ee80;
                    }
                    record->unk5c = 0.0f;
                }
            }
        }
    }
}

// FUNCTION: RALLYTROPHY 0x48ffd0
RecordEntry* RecordTable::FUN_0048ffd0(int a, int b, int c, int d)
{
    int group = a;
    if (group) {
        group = group * 11 + 2;
    }
    return (RecordEntry*)(records + (((group + b + 3) * 23 + d) * 10 + c) * 0x60);
}

// FUNCTION: RALLYTROPHY 0x490010
RecordEntry* RecordTable::FUN_00490010(int a, int b, int c, int d)
{
    int group = a;
    if (group) {
        group = group * 11 + 2;
    }
    return (RecordEntry*)(records + (((group + b) * 23 + d) * 10 + c) * 0x60);
}

// NONMATCHING: RALLYTROPHY 0x490050
// Insert by the 0x54 time into the list of the stage and the list of the car class.
// Same structure and size as the original; register allocation differs (key/record/first end up in other registers).
int RecordTable::FUN_00490050(ScoreRecord* record, int a, int b)
{
    int result = 0;
    int i = 0;
    int list = a ? a * 11 + 2 : 0;
    list += b + 3;
    int key = record->splits[9];
    ScoreRecord* first = (ScoreRecord*)records + list * 230;
    int* time = &first->splits[9];
    for (; i < 10; i++, time += 0x18) {
        if (key < *time) {
            dirty = 1;
            if (!i) {
                result = 1;
            }
            for (int j = 9; j > i; j--) {
                first[j] = first[j - 1];
            }
            ((ScoreRecord*)records)[list * 230 + i] = *record;
            break;
        }
    }
    list = list * 23 + record->unk1f;
    i = 0;
    first = (ScoreRecord*)records + list * 10;
    time = &first->splits[9];
    for (; i < 10; i++, time += 0x18) {
        if (key < *time) {
            dirty = 1;
            for (int j = 9; j > i; j--) {
                first[j] = first[j - 1];
            }
            ((ScoreRecord*)records)[list * 10 + i] = *record;
            break;
        }
    }
    return result;
}

// NONMATCHING: RALLYTROPHY 0x4901b0
// Insert by the 0x58 time.
// Same structure and size as the original; register allocation differs (key/record/first end up in other registers).
int RecordTable::FUN_004901b0(ScoreRecord* record, int a, int b)
{
    int result = 0;
    int i = 0;
    int list = a ? a * 11 + 2 : 0;
    list += b + 3;
    int key = record->time;
    ScoreRecord* first = (ScoreRecord*)records + list * 230;
    int* time = &first->time;
    for (; i < 10; i++, time += 0x18) {
        if (key < *time) {
            dirty = 1;
            if (!i) {
                result = 1;
            }
            for (int j = 9; j > i; j--) {
                first[j] = first[j - 1];
            }
            ((ScoreRecord*)records)[list * 230 + i] = *record;
            break;
        }
    }
    list = list * 23 + record->unk1f;
    i = 0;
    first = (ScoreRecord*)records + list * 10;
    time = &first->time;
    for (; i < 10; i++, time += 0x18) {
        if (key < *time) {
            dirty = 1;
            for (int j = 9; j > i; j--) {
                first[j] = first[j - 1];
            }
            ((ScoreRecord*)records)[list * 10 + i] = *record;
            break;
        }
    }
    return result;
}

// NONMATCHING: RALLYTROPHY 0x490310
// Insert by the 0x58 time, without the +3 list offset.
// Same structure and size as the original; register allocation differs (key/record/first end up in other registers).
int RecordTable::FUN_00490310(ScoreRecord* record, int a, int b)
{
    int result = 0;
    int i = 0;
    int list = a ? a * 11 + 2 : 0;
    list += b;
    int key = record->time;
    ScoreRecord* first = (ScoreRecord*)records + list * 230;
    int* time = &first->time;
    for (; i < 10; i++, time += 0x18) {
        if (key < *time) {
            dirty = 1;
            if (!i) {
                result = 1;
            }
            for (int j = 9; j > i; j--) {
                first[j] = first[j - 1];
            }
            ((ScoreRecord*)records)[list * 230 + i] = *record;
            break;
        }
    }
    list = list * 23 + record->unk1f;
    i = 0;
    first = (ScoreRecord*)records + list * 10;
    time = &first->time;
    for (; i < 10; i++, time += 0x18) {
        if (key < *time) {
            dirty = 1;
            for (int j = 9; j > i; j--) {
                first[j] = first[j - 1];
            }
            ((ScoreRecord*)records)[list * 10 + i] = *record;
            break;
        }
    }
    return result;
}


// The record of the oldest file version (0x68 bytes).
struct OldScoreRecord {
    char name[0x1e];                   // 0x00
    int carClass;                      // 0x20: index into DAT_004ff354
    char unk24;                        // 0x24: car class
    SYSTEMTIME date;                   // 0x28
    int splits[10];                    // 0x38
    int time;                          // 0x60
    int unk64;
};

// NONMATCHING: RALLYTROPHY 0x490470
// 918 vs 929 bytes. Structure, loops and frame are the same; the register assignment differs (this in ebp instead of ebx),
// so the loop counters and several operands are swapped.
// Loads the highscore file: the current format is the whole table, enciphered; the older formats are converted.
void RecordTable::FUN_00490470(const char* file)
{
    if (!File::Exists(file)) {
        dirty = 1;
        return;
    }
    File stream;
    stream.Open(file, 0x81);
    int magic;
    stream.Read(&magic, 4);
    if (magic == DAT_004ff350) {
        stream.Read(this, 0x133440);
        Cipher cipher;
        cipher.Decrypt((DWORD*)this, DAT_004ff3d4, 0x4cd10);
    } else if (magic == DAT_004ff34c) {
        dirty = 1;
    } else if (magic == DAT_004ff348) {
        dirty = 1;
        for (int g = 0; g < 5; g++) {
            int base = g ? g * 11 + 2 : 0;
            for (int h = 0; h < 13; h++) {
                for (int k = 0; k < 23; k++) {
                    for (int e = 0; e < 10; e++) {
                        if (g > 0 && h > 10) {
                            ScoreRecord skipped;
                            stream.Read(&skipped, 0x60);
                        } else {
                            ScoreRecord* record = (ScoreRecord*)(records + (((base + h) * 23 + k) * 10 + e) * 0x60);
                            stream.Read(record, 0x60);
                            record->unk1e = DAT_004ff354[(char)record->unk1e];
                        }
                    }
                }
            }
        }
    } else if (magic == DAT_004ff344) {
        dirty = 1;
        for (int g = 0; g < 5; g++) {
            int base = (g ? g * 11 + 2 : 0) + 3;
            for (int h = 0; h < 10; h++) {
                char counts[22];
                memset(counts, 0, sizeof counts);
                for (int e = 0; e < 10; e++) {
                    OldScoreRecord old;
                    stream.Read(&old, 0x68);
                    if (g == 0 || h < 8) {
                        ScoreRecord record;
                        memcpy(record.name, old.name, 0x1e);
                        record.unk1e = DAT_004ff354[old.carClass];
                        record.unk1f = old.unk24;
                        record.date = old.date;
                        memcpy(record.splits, old.splits, sizeof(old.splits));
                        record.time = old.time;
                        record.unk5c = 0.0f;
                        *(ScoreRecord*)(records + (((base + h) * 23) * 10 + e) * 0x60) = record;
                        char* count = &counts[record.unk1f - 1];
                        unsigned char index = *count;
                        *(ScoreRecord*)(records + (((base + h) * 23 + record.unk1f) * 10 + index) * 0x60) = record;
                        *count = index + 1;
                    }
                }
            }
        }
    }
    stream.Close();
}

// FUNCTION: RALLYTROPHY 0x490820
// Saves the table (when dirty) as the current file version, enciphered.
void RecordTable::FUN_00490820(const char* file)
{
    if (dirty) {
        File stream;
        stream.Open(file, 0x82);
        stream.Write(&DAT_004ff350, 4);
        BYTE* copy = new BYTE[0x133440];
        memcpy(copy, this, 0x133440);
        Cipher cipher;
        cipher.Encrypt((DWORD*)copy, DAT_004ff3d4, 0x4cd10);
        stream.Write(copy, 0x133440);
        delete copy;
        stream.Close();
        dirty = 0;
    }
}

// Module end: the guard-bit initialiser of the STL header statics and its
// atexit registration, which VC6 emits for every file including <string>/<vector>.

// FUNCTION: RALLYTROPHY 0x490920
// SYMBOL: _$E23
typedef void Generated_490920();

// FUNCTION: RALLYTROPHY 0x490940
// SYMBOL: _$E22
typedef void Generated_490940();
