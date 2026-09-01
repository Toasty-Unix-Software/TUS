default rel
	vblendpd xmm0, xmm4, xmm2, 0xc6
	vblendpd xmm0, xmm2, 0xc6
	vblendpd xmm3, xmm4, xmm6, 0xff
	vblendpd ymm1, ymm1, ymm5, 0x19
	vblendpd ymm1, ymm5, 0x19
	vblendpd ymm2, ymm0, ymm0, 0xb3
	vblendpd xmm14, xmm13, xmm13, 0xa6
	vblendpd ymm14, ymm15, ymm10, 0xa
	vblendpd xmm2, xmm0, oword [eax+1], 0xa8
	vblendpd xmm0, xmm7, oword [eax+64], 0x21
	vblendpd ymm6, ymm2, yword [eax+1], 0xd7
	vblendpd ymm5, ymm0, yword [eax+64], 0x31
	vblendpd xmm1, xmm6, [0x5c7], 0x1f
	vblendpd ymm5, ymm2, [0xc1f], 0x80
