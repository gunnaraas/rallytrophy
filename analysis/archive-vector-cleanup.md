# Archive-cache vector cleanup

`FUN_004a4cb0` walks the archive-cache pointer range at `DAT_0056bbac` to
`DAT_0056bbb0`. For each non-null cache pointer it destroys the
`FileArchiveCache` and releases the object allocation. Afterward it sets the
vector's end pointer to its begin pointer while retaining the vector storage.
The body and its call sequence are observed in the shipped executable
(Evidence `ev_145f9623f741c1a865180898e7e444b5f4d7be465a934338ebf696b7bb4cb77c`).

The behavior reconstruction is in `src/file_archive.cpp` at `0x4a4cb0`. It is
marked `NONMATCHING` (88 compiled bytes versus 133 original bytes); the vector
template cleanup sequence and register allocation remain to be reproduced.
Evidence bundle: `evidence-2026-10-22-archive-vector-cleanup.json`.
