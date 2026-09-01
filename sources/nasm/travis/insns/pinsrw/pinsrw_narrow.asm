	pinsrw mm7, [0xb21], 0x12
	pinsrw mm1, [0x40d], 0x5d
	pinsrw mm5, ax, 0xb
	pinsrw mm7, ax, 0x2f
	pinsrw mm0, edx, 0x5d
	pinsrw mm2, esi, 0x46
	pinsrw xmm1, si, 0x46
	pinsrw xmm1, dx, 0x64

%ifdef ERROR
	pinsrw mm0, r13w, 0x4e
	pinsrw mm1, r11d, 0x41
	pinsrw xmm11, r10w, 0x3
%endif
