#ifndef RT_FILE_ARCHIVE_H
#define RT_FILE_ARCHIVE_H

// Patch 1.01 archive cache; see docs/analysis/file-archive-cache.md.
// Header bytes +0x10..+0x1b are read directly from the archive.
class FileArchiveCache {
public:
    FileArchiveCache();               // original: FUN_004a4b10
    ~FileArchiveCache();              // original: FUN_004a4bd0

    std::string m_name;               // +0x00
    unsigned char m_header[8];        // +0x10: untouched by constructor
    unsigned int m_entryCount;        // +0x18
    unsigned char* m_entries;         // +0x1c: packed 0x35-byte records
};

// Global archive-cache vector state initialized by FUN_004a4c60.
void __fastcall FUN_004a4c60(unsigned int value);
void __stdcall FUN_004a4c80(void);

#endif
