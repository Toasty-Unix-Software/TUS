default rel
	vblendvpd xmm6, xmm2, xmm6, xmm3
	vblendvpd xmm6, xmm6, xmm3
	vblendvpd xmm2, xmm1, oword [0x682], xmm3
	vblendvpd ymm0, ymm6, ymm3, ymm1
	vblendvpd ymm0, ymm3, ymm1
	vblendvpd ymm4, ymm0, ymm0, ymm5
	vblendvpd xmm14, xmm8, xmm13, xmm11
	vblendvpd ymm12, ymm11, ymm10, ymm8
	vblendvpd xmm7, xmm5, oword [eax+1], xmm4
	vblendvpd xmm3, xmm7, oword [eax+64], xmm7
	vblendvpd ymm6, ymm1, yword [eax+1], ymm2
	vblendvpd ymm7, ymm6, yword [eax+64], ymm5
	vblendvpd xmm6, xmm2, [0x31a], xmm6
	vblendvpd ymm3, ymm4, [0xe68], ymm7
