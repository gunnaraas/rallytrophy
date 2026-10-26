# Doubly linked list helpers

`List::AddTail` at `0x4aecf0` reuses a node from the free list when possible,
otherwise allocates a 12-byte node, then links it at the tail and updates the
list counts. It matches the 105-byte shipped function (Evidence
`ev_f316c5a9923a67635fa8a5335d884dfe071d2f029a92e91c845a72f1c3441b78`).

`List::RemoveNode` at `0x414490` unlinks the supplied node, pushes it onto the
free list, and adjusts both counters. It matches all 76 bytes (Evidence
`ev_43638a515a608910b87edd1dd712414813fe06bdbf682ca3d32f17f5de2eba71`).
Both pointer-list specializations are in `src/unk_4bbdf0.cpp`.
