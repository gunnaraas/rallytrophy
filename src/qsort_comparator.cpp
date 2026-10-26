// PE transcription of the qsort comparator referenced as data at 0x482850.

__declspec(naked)
// Inference from the PE instruction sequence: compare entry sums, then a two-byte-unrolled
// narrow string key; details are recorded in analysis/qsort-comparator.md.
// FUNCTION: RALLYTROPHY 0x482850
int __cdecl FUN_00482850(const void*, const void*)
{
    __asm {
        _emit 08bh
        _emit 044h
        _emit 024h
        _emit 004h
        _emit 08bh

        _emit 04ch
        _emit 024h
        _emit 008h
        _emit 053h
        _emit 055h

        _emit 08bh
        _emit 028h
        _emit 08bh
        _emit 09dh
        _emit 050h

        _emit 01ah
        _emit 000h
        _emit 000h
        _emit 056h
        _emit 08bh

        _emit 031h
        _emit 033h
        _emit 0c9h
        _emit 033h
        _emit 0d2h

        _emit 085h
        _emit 0dbh
        _emit 07eh
        _emit 02dh
        _emit 057h

        _emit 08bh
        _emit 0fdh
        _emit 08dh
        _emit 086h
        _emit 028h

        _emit 01ah
        _emit 000h
        _emit 000h
        _emit 02bh
        _emit 0feh

        _emit 003h
        _emit 00ch
        _emit 007h
        _emit 003h
        _emit 010h

        _emit 083h
        _emit 0c0h
        _emit 004h
        _emit 04bh
        _emit 075h

        _emit 0f5h
        _emit 03bh
        _emit 0cah
        _emit 05fh
        _emit 07eh

        _emit 009h
        _emit 05eh
        _emit 05dh
        _emit 0b8h
        _emit 001h

        _emit 000h
        _emit 000h
        _emit 000h
        _emit 05bh
        _emit 0c3h

        _emit 07dh
        _emit 007h
        _emit 05eh
        _emit 05dh
        _emit 083h

        _emit 0c8h
        _emit 0ffh
        _emit 05bh
        _emit 0c3h
        _emit 08bh

        _emit 0c5h
        _emit 08ah
        _emit 010h
        _emit 08ah
        _emit 01eh

        _emit 08ah
        _emit 0cah
        _emit 03ah
        _emit 0d3h
        _emit 075h

        _emit 020h
        _emit 084h
        _emit 0c9h
        _emit 074h
        _emit 016h

        _emit 08ah
        _emit 050h
        _emit 001h
        _emit 08ah
        _emit 05eh

        _emit 001h
        _emit 08ah
        _emit 0cah
        _emit 03ah
        _emit 0d3h

        _emit 075h
        _emit 010h
        _emit 083h
        _emit 0c0h
        _emit 002h

        _emit 083h
        _emit 0c6h
        _emit 002h
        _emit 084h
        _emit 0c9h

        _emit 075h
        _emit 0dch
        _emit 05eh
        _emit 05dh
        _emit 033h

        _emit 0c0h
        _emit 05bh
        _emit 0c3h
        _emit 05eh
        _emit 01bh

        _emit 0c0h
        _emit 05dh
        _emit 083h
        _emit 0d8h
        _emit 0ffh

        _emit 05bh
        _emit 0c3h
    }
}
