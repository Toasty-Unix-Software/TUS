default rel
	pinsrw mm7, [0xb21], 0x12
	pinsrw mm1, [0x40d], 0x5d
	pinsrw mm5, ax, 0xb
	pinsrw mm7, ax, 0x2f
	pinsrw mm0, edx, 0x5d
	pinsrw mm2, esi, 0x46
	pinsrw xmm1, si, 0x46
	pinsrw xmm1, dx, 0x64
	pinsrw mm0, r13w, 0x4e
	pinsrw mm1, r11d, 0x41
	pinsrw xmm11, r10w, 0x3
	pinsrw mm4, [eax+1], 0x55
	pinsrw mm4, [eax+64], 0x6c
	pinsrw mm1, word [eax+1], 0x4c
	pinsrw mm4, word [eax+64], 0x20
	pinsrw xmm4, [eax+1], 0x4c
	pinsrw xmm6, [eax+64], 0x69
	pinsrw xmm7, word [eax+1], 0x3b
	pinsrw xmm6, word [eax+64], 0x3c
	pinsrw mm1, [0xeb1], 0x72
	pinsrw xmm1, [0xa48], 0x70
