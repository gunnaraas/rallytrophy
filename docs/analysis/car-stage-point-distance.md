# Car stage-point distance (`0x45b390`)

## Evidence

REA instruction evidence `ev_482eda42dbfbd7e19e133ac34ae46d58a20274d24bb34531c85f16e68882584a`
identifies the 1,380-byte `Car::FUN_0045b390` body. At the stage-point distance test,
the target loads the y component with an integer `mov` before loading x onto the x87
stack, then stores the z difference back to `[esp+0x1c]` before squaring it.

## Source match

`StagePointCopy` copies y through an integer lvalue before x and z. The distance test
subtracts x and y explicitly, computes the z delta into a float temporary, and stores
that value through a volatile alias to `d.z`. This produces the target's load order and
x87 spill while preserving the rest of the function's source structure.

The focused matcher reports an exact 1,380-byte body with 25 relocations. The source
file check also passes with the remaining functions unchanged.
