# ABI Register Cheat Sheets

The following are ABI register usage notes accumulated with development and porting of musl to various archs:

# aarch64

* x30 lr
* x31 zr or pc

# arm

* r0-r3 args/ret
* r4-r11 saved
* r12 temp (ip scratch)
* r13 sp
* r14 lr
* r15 pc

# mips

* $0 zero
* $1 at (assembler temp)
* $2-$3 ret (aka v0-v1)
* $4-$7 args (aka a0-a3)
* $8-$15 temp (aka t0-t7)
* $16-$23 saved (aka s0-s7)
* $24 temp (aka t8)
* $25 function call addr (aka t9)
* $26-$27 kernel use
* $28 gp, call-clobbered
* $29 sp
* $30 fp
* $31 ra

Source: http://www.inf.ed.ac.uk/teaching/courses/car/Notes/slide03.pdf

# or1k (OpenRISC)

* r0 zero
* r1 sp
* r2 fp
* r3-r8 args
* r9 lr
* r11,r12 retval (lo,hi)
* r10 thread pointer
* r14-r30(even) saved
* r13-r31(odd) temp

Source: openrisc-arch-1.1-rev0.pdf, p.335
