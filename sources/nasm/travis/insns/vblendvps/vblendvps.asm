default rel
	vblendvps xmm5, xmm2, xmm6, xmm6
	vblendvps xmm5, xmm6, xmm6
	vblendvps xmm3, xmm2, oword [0x562], xmm4
	vblendvps ymm3, ymm6, yword [0x6a2], ymm7
	vblendvps ymm3, yword [0x6a2], ymm7
	vblendvps ymm6, ymm4, ymm4, ymm1
	vblendvps xmm15, xmm14, xmm13, xmm13
	vblendvps ymm11, ymm12, ymm11, ymm13
	vblendvps xmm7, xmm3, oword [eax+1], xmm1
	vblendvps xmm6, xmm6, oword [eax+64], xmm1
	vblendvps ymm7, ymm0, yword [eax+1], ymm2
	vblendvps ymm2, ymm6, yword [eax+64], ymm0
	vblendvps xmm5, xmm5, [0x9fc], xmm6
	vblendvps ymm1, ymm0, [0xff4], ymm1
