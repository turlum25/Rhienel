/*
 * boot stuff
 * don't be a dumb guy and touch the 64-byte offsets
 * or you will be executed
 * 2026 - 22 - 07
 */

.section .text.boot, "ax"
.global _start
.global primary_entry

_start:
    /* Official Linux Boot Header layout frame - DO NOT MODIFY */
	// well sure mr. linus torvalds, sure...
    b       primary_entry       /* offset 0x00: jump past the boilerplate header */
    .int    0                   /* offset 0x04: reserved */
    .quad   0                   /* offset 0x08: text load point */
    .quad   0                   /* offset 0x10: total kernel footprint size */
    .quad   0                   /* offset 0x18: execution parameter flags */
    .quad   0                   /* offset 0x20: reserved */
    .quad   0                   /* offset 0x28: reserved */
    .quad   0                   /* offset 0x30: reserved */
    .ascii  "ARM\x64"            /* offset 0x38: magic verification token string */
    .int    0                   /* offset 0x3C: reserved */

primary_entry:
    // dear me: i don't know what the hell this is
	// but it works at least :shrug:
    mrs     x0, mpidr_el1
    and     x0, x0, #0xFF
    cbz     x0, .Lmaster_init

.Lsecondary_sleep:
    wfe                         // shove useless extra processor cores here
    b       .Lsecondary_sleep

.Lmaster_init:
    // establish raw stack bounds right under entry memory block addresses (heck)
    ldr     x0, =stack_top
    mov     sp, x0

    // jump directly into this thing in main.c :3
    bl      MainCoolThingy

.Lpanic_hang:
    wfe                         // backup fallback loop if execution drops out from Squidward Community College
    b       .Lpanic_hang

// this thing that Google Gemini told me to put to prevent
// rawK from crashing
// i guess it'll fix it :shrug:
// Re: It works to this day, thank you mr coolio gemini
.balign 16
.section .bss
.space 16384
stack_top:
