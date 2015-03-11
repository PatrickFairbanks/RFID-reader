	// .file	"C:/Users/Andrew/Documents/GitHub/RFID-reader/XInC2_uC_firmware/bin/RFID-reader.opt"
//	.section	//.text,"xr"
//align
@ = (@ + 1-1)  & -1
sxc_memset:                             // @memset
// BB#0:
	sub	r1, r4, 0x0
	bc	EQ, BB0_3
// BB#1:
	add	r0, r2, 0x0
BB0_2:                                  // %.lr.ph
                                        // =>This Inner Loop Header: Depth=1
	st	r3, r0, 0x0
	add	r0, r0, 0x1
	add	r4, r4, 0xffff
	//cmp	r4, 0x0
	bc	ZC, BB0_2
BB0_3:                                  // %._crit_edge
	add	r0, r2, 0x0
	jsr	r6, r6

//align
@ = (@ + 1-1)  & -1
sxc_memcpy:                             // @memcpy
// BB#0:
	sub	r1, r4, 0x0
	bc	EQ, BB1_3
// BB#1:
	add	r0, r2, 0x0
BB1_2:                                  // %.lr.ph
                                        // =>This Inner Loop Header: Depth=1
	ld	r1, r3, 0x0
	st	r1, r0, 0x0
	add	r3, r3, 0x1
	add	r0, r0, 0x1
	add	r4, r4, 0xffff
	//cmp	r4, 0x0
	bc	ZC, BB1_2
BB1_3:                                  // %._crit_edge
	add	r0, r2, 0x0
	jsr	r6, r6

//align
@ = (@ + 1-1)  & -1
sxc_memmove:                            // @memmove
// BB#0:
	sub	r1, r4, 0x0
	bc	EQ, BB2_3
// BB#1:
	add	r0, r2, 0x0
BB2_2:                                  // %.lr.ph.i
                                        // =>This Inner Loop Header: Depth=1
	ld	r1, r3, 0x0
	st	r1, r0, 0x0
	add	r3, r3, 0x1
	add	r0, r0, 0x1
	add	r4, r4, 0xffff
	//cmp	r4, 0x0
	bc	ZC, BB2_2
BB2_3:                                  // %memcpy.exit
	add	r0, r2, 0x0
	jsr	r6, r6

//sxc_main
//align
@ = (@ + 1-1)  & -1
sxc_main:                               // @main
// BB#0:
	add	sp, sp, 0x3
	st	r6, sp, 0xfffd          // 1-byte Folded Spill
	inp	r0, 0x26
	and	r0, r0, 0xffdf
	outp	r0, 0x26
	inp	r0, 0x3
	inp	r0, 0x3
	inp	r0, 0x26
	ior	r0, r0, 0x20
	outp	r0, 0x26
	inp	r0, 0x3
	inp	r0, 0x3
	inp	r0, 0x26
	and	r0, r0, 0xffdf
	outp	r0, 0x26
	inp	r0, 0x3
	inp	r0, 0x3
	inp	r0, 0x26
	and	r0, r0, 0xffdf
	outp	r0, 0x26
	inp	r0, 0x3
	inp	r0, 0x3
	inp	r0, 0x26
	and	r0, r0, 0xffdf
	outp	r0, 0x26
	inp	r0, 0x3
	inp	r0, 0x3
	inp	r0, 0x26
	and	r0, r0, 0xffdf
	outp	r0, 0x26
	inp	r0, 0x3
	inp	r0, 0x3
	inp	r0, 0x26
	ior	r0, r0, 0x20
	outp	r0, 0x26
	inp	r0, 0x3
	inp	r0, 0x3
	inp	r0, 0x26
	ior	r0, r0, 0x20
	outp	r0, 0x26
	inp	r0, 0x3
	inp	r0, 0x3
	inp	r0, 0x26
	and	r0, r0, 0xffdf
	outp	r0, 0x26
	inp	r0, 0x3
	inp	r0, 0x3
	inp	r0, 0x26
	ior	r0, r0, 0x20
	outp	r0, 0x26
	inp	r0, 0x3
	inp	r0, 0x3
BB3_1:                                  // =>This Inner Loop Header: Depth=1
	inp	r0, 0x26
	bic	r0, r0, 0x4
	//cmp	r0, 0x0
	bc	VS, BB3_1
// BB#2:                                // %.preheader.i
	inp	r0, 0x3
	inp	r0, 0x3
	inp	r3, 0x26
	inp	r0, 0x3
	inp	r0, 0x3
	inp	r0, 0x26
	and	r0, r0, 0xfff8
	rol	r0, r0, 0xd
	and	r0, r0, 0x1ffe
	st	r0, sp, 0xfffe          // 1-byte Folded Spill
	inp	r0, 0x3
	inp	r0, 0x3
	inp	r0, 0x26
	and	r0, r0, 0xfffc
	rol	r0, r0, 0xe
	and	r0, r0, 0x3ffc
	st	r0, sp, 0xffff          // 1-byte Folded Spill
	inp	r0, 0x3
	inp	r0, 0x3
	inp	r0, 0x26
	inp	r1, 0x3
	inp	r1, 0x3
	inp	r4, 0x26
	and	r0, r0, 0xfffe
	rol	r0, r0, 0xf
	and	r5, r0, 0x7ff8
	inp	r0, 0x3
	inp	r0, 0x3
	inp	r0, 0x26
	and	r0, r0, 0x7fff
	rol	r0, r0, 0x1
	and	r6, r0, 0xffe0
	inp	r0, 0x3
	inp	r0, 0x3
	inp	r0, 0x26
	and	r0, r0, 0x3fff
	rol	r0, r0, 0x2
	and	r0, r0, 0xffc0
	inp	r1, 0x3
	inp	r1, 0x3
	inp	r1, 0x26
	and	r1, r1, 0x1fff
	rol	r1, r1, 0x3
	and	r2, r1, 0xff80
	inp	r1, 0x3
	inp	r1, 0x3
BB3_3:                                  // =>This Inner Loop Header: Depth=1
	inp	r1, 0x26
	bic	r1, r1, 0x4
	//cmp	r1, 0x0
	bc	VC, BB3_3
// BB#4:                                // %uart_read_byte.exit
	ld	r1, sp, 0xfffe          // 1-byte Folded Reload
	and	r1, r1, 0x2
	and	r3, r3, 0xfff0
	rol	r3, r3, 0xc
	and	r3, r3, 0x1
	ior	r1, r1, r3
	ld	r3, sp, 0xffff          // 1-byte Folded Reload
	and	r3, r3, 0x4
	ior	r1, r1, r3
	and	r3, r5, 0x8
	ior	r1, r1, r3
	and	r3, r4, 0x10
	ior	r1, r1, r3
	and	r3, r6, 0x20
	ior	r1, r1, r3
	and	r0, r0, 0x40
	ior	r0, r1, r0
	and	r1, r2, 0x80
	add	r1, r1, r0
	//APP
	//Asserting number to R1 for XPD_EchoUnsignedDec
	jsr	r6, XPD_EchoUnsignedDec
	//NO_APP
	mov	r0, 0x0
	ld	r6, sp, 0xfffd          // 1-byte Folded Reload
	sub	sp, sp, 0x3
	jsr	r6, r6


