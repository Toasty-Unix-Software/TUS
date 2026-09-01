default rel
	vblendmps xmm6, xmm2, xmm3
	vblendmps xmm3, xmm7, xmm1
	vblendmps ymm2, ymm6, ymm2
	vblendmps ymm3, ymm3, ymm0
	vblendmps zmm1, zmm0, zmm4
	vblendmps zmm1, zmm6, zmm3
	vblendmps xmm8, xmm12, xmm14
	vblendmps ymm11, ymm12, ymm12
	vblendmps zmm15, zmm8, zmm13
	vblendmps xmm21, xmm25, xmm26
	vblendmps ymm31, ymm25, ymm22
	vblendmps zmm22, zmm27, zmm24
	vblendmps xmm7{k2}, xmm6, oword [0x385]
	vblendmps ymm7{k3}, ymm4, yword [0x8c0]
	vblendmps zmm6{k2}, zmm7, zmm5
	vblendmps xmm6{k2}{z}, xmm6, xmm6
	vblendmps ymm2{k3}{z}, ymm6, ymm1
	vblendmps zmm0{k5}{z}, zmm7, zword [0xef9]
	vblendmps xmm1, xmm2, oword [eax+1]
	vblendmps xmm5, xmm3, oword [eax+64]
	vblendmps ymm5, ymm6, yword [eax+1]
	vblendmps ymm7, ymm0, yword [eax+64]
	vblendmps zmm7, zmm4, zword [eax+1]
	vblendmps zmm3, zmm5, zword [eax+64]
	vblendmps xmm6, xmm0, [0x721]
	vblendmps ymm1, ymm2, [0xa06]
	vblendmps zmm2, zmm5, [0x6af]
