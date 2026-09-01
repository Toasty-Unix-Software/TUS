default rel
	vblendps xmm4, xmm4, oword [0x99f], 0x25
	vblendps xmm4, oword [0x99f], 0x25
	vblendps xmm7, xmm0, xmm4, 0x7
	vblendps ymm2, ymm2, ymm7, 0x4b
	vblendps ymm2, ymm7, 0x4b
	vblendps ymm7, ymm2, yword [0xcfe], 0x42
	vblendps xmm12, xmm12, xmm10, 0x5d
	vblendps ymm13, ymm9, ymm12, 0x4a
	vblendps xmm1, xmm1, oword [eax+1], 0x55
	vblendps xmm1, xmm4, oword [eax+64], 0xe9
	vblendps ymm1, ymm4, yword [eax+1], 0x3f
	vblendps ymm3, ymm3, yword [eax+64], 0xc1
	vblendps xmm4, xmm5, [0xf1e], 0xc9
	vblendps ymm4, ymm1, [0xe25], 0xa1
