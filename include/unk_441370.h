/* TrackSelectMenu (0x441370-0x445e7f): the "Select Track" screen (country list, special stage list, Continue / Back),
 * a MenuScreen subclass embedded in the Game object at +0x3aec (0x1ef8 bytes, vtable 0x4e18b4). Map:
 * analysis/range-441370.md section 1. Include after unk_481390.h, rallytrophy.h and unk_450ee0.h (MenuScreen,
 * MenuGame). The same object is seen as `StageTable` in unk_481390.h (GameView::stageTable); that view is left alone. */
#ifndef UNK_441370_H
#define UNK_441370_H

// The per-rally info block (0x5e8 bytes, five of them in the screen). Constructor 0x4414f0 and destructor 0x441620
// are the implicit ones; DrawRally (0x445870) takes it by value.
struct RallyInfo {
    std::string name;                  // 0x00: text 0xb5 + rally (RUSSIA ...)
    std::string description;           // 0x10: text 0xba + rally
    int numStages;                     // 0x20: NumberOfStages
    int numNovice;                     // 0x24: NumberOfNoviceStages
    int numIntermediate;               // 0x28: NumberOfIntermediateStages
    int numExpert;                     // 0x2c: NumberOfExpertStages
    int numArcade;                     // 0x30: NumberOfArcadeStages
    int firstArcade;                   // 0x34: FirstArcadeStage - 1
    float length[12];                  // 0x38: km (miles when the miles flag is set)
    std::string stageName[12];         // 0x68
    std::string lengthText[12];        // 0x128
    std::string timeText[12];          // 0x1e8
    std::string surfaceText[12];       // 0x2a8
    std::string recordTime[12];        // 0x368
    std::string recordSpeed[12];       // 0x428
    std::string recordTopSpeed[12];    // 0x4e8
    BYTE unk5a8[12];                   // 0x5a8: per-stage record car class (LoadRallyData)
    Sprite* photo;                     // 0x5b4: Sprite+0 InfoPhotoWire.tga, Sprite+4 InfoPhotoFull.tga
    Sprite* stageInfo[12];             // 0x5b8: StageInfo<k+1>.tga
};

// A std::string (same layout) whose constructor stays out of line (_Tidy, 0x402700) and whose destructor is expanded
// inline: the first string of the TrackSelectMenu constructor is built that way (VC6 inline budget). str() gives the
// ordinary std::string view for the other methods.
class TS_Str {
public:
    TS_Str(const std::allocator<char>& al = std::allocator<char>()) : alloc(al) { Tidy(false); }
    ~TS_Str()
    {
        if (ptr == 0) {
        } else if (Refcnt(ptr) == 0 || Refcnt(ptr) == 255) {
            alloc.deallocate(ptr - 1, res + 2);
        } else {
            --Refcnt(ptr);
        }
        ptr = 0, len = 0, res = 0;
    }
    static unsigned char& Refcnt(const char* u) { return ((unsigned char*)u)[-1]; }
    std::string& str() { return *(std::string*)this; }
    void Tidy(bool built);             // 0x402700
    std::allocator<char> alloc;
    char* ptr;
    unsigned int len;
    unsigned int res;
};

extern ConfigKey DAT_004f92c8[];       // info.ini keys (66 entries, Setup)

class TrackSelectMenu : public MenuScreen {
public:
    TrackSelectMenu();                 // 0x4413b0
    virtual ~TrackSelectMenu();        // 0x441760 (deleting form 0x4414d0)
    virtual void Setup(MenuGame* game); // 0x4418d0
    virtual void Release();            // 0x443480 (own implementation)
    virtual void Slot3(int unused);    // 0x4414c0 (empty, folded with the other screens' copies)
    void LoadRallyData();              // 0x442b20
    int GetRallyCount();               // 0x443580
    int GetStageCount(int rally);      // 0x443620
    int GetArcadeStageCount(int rally); // 0x4436b0
    int Open(MenuGame* game, int mode); // 0x443700
    int Run();                         // 0x443790
    void DrawDetails();                // 0x444c40
    void DrawDescription();            // 0x4450e0
    void BuildRects();                 // 0x4451c0
    void DrawRally(RallyInfo info);    // 0x445870 (ret 0x5e8: the copy is destroyed by the callee)
    void DrawTrackBook();              // 0x445c90
    // MenuScreen methods, declared here like the sibling screens do
    void DrawSelector(TextRenderer* text, std::string* value, float x, float y, float width, bool selected, int current,
                      int lo, int hi); // 0x438dc0
    void FUN_00438c40(int* value, int delta, int lo, int hi); // 0x438c40
    void FUN_00438d00();               // 0x438d00

    TS_Str title;                      // 0x20 (use title.str() for std::string methods): text 0x101 "Select Track"
    int unk30;                         // 0x30
    int unk34;                         // 0x34
    float textScaleX;                  // 0x38: screen width / 1024 * 32
    float textScaleY;                  // 0x3c: screen height / 768 * 16
    int unk40;                         // 0x40: -1 after Setup
    std::string unk44;                 // 0x44: text 0x17 "Continue"
    std::string unk54;                 // 0x54: text 0x18 "Back"
    std::string unk64;                 // 0x64: text 0x19 "Country"
    std::string unk74;                 // 0x74: text 0x1a "Special Stage"
    std::string labels[12];            // 0x84: Location, Length, Time, Surface, Stage Record, "" ...
    Sprite* trackBook;                 // 0x144: data/menu/TrackBook.tga
    RallyInfo rally[5];                // 0x148
    int curRally;                      // 0x1ed0
    int curStage;                      // 0x1ed4
    int lastStageArcade;               // 0x1ed8
    int lastStage;                     // 0x1edc
    int mode;                          // 0x1ee0: 0 = rally stages, else special/arcade stages
    int rallyCount;                    // 0x1ee4: constant 3
    int availableRallies;              // 0x1ee8
    int animStartTick;                 // 0x1eec
    int slideTick;                     // 0x1ef0
    int slideDir;                      // 0x1ef4
};

#endif
