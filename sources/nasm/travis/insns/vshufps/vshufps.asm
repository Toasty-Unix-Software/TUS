default rel
	vshufps xmm5, xmm4, xmm0, 0x1f
	vshufps xmm5, xmm0, 0x1f
	vshufps xmm1, xmm3, oword [0x6da], 0xb0
	vshufps ymm1, ymm5, yword [0xb6d], 0xd0
	vshufps ymm1, yword [0xb6d], 0xd0
	vshufps ymm1, ymm1, ymm5, 0xf0
	vshufps xmm2, xmm3, xmm3, 0xfe
	vshufps xmm2, xmm3, 0xfe
	vshufps xmm2, xmm4, xmm4, 0x48
	vshufps ymm4, ymm4, ymm3, 0x56
	vshufps ymm4, ymm3, 0x56
	vshufps ymm4, ymm0, ymm0, 0xa3
	vshufps xmm12, xmm10, xmm9, 0x6b
	vshufps ymm10, ymm13, ymm11, 0xc5
	vshufps xmm9, xmm15, xmm14, 0x71
	vshufps ymm10, ymm10, ymm13, 0x7c
	vshufps xmm19, xmm17, xmm22, 0x1
	vshufps ymm27, ymm28, ymm16, 0xa7
	vshufps xmm17, xmm21, xmm22, 0x13
	vshufps ymm25, ymm16, ymm18, 0x3c
	vshufps xmm4{k1}, xmm7, xmm1, 0x17
	vshufps ymm5{k3}, ymm0, ymm5, 0x1f
	vshufps zmm0{k5}, zmm5, zmm3, 0xf
	vshufps xmm5{k5}{z}, xmm6, oword [0x6de], 0x3
	vshufps ymm6{k2}{z}, ymm5, ymm2, 0x5d
	vshufps zmm2{k2}{z}, zmm4, zmm7, 0xa4
	vshufps xmm7, xmm4, oword [eax+1], 0x7
	vshufps xmm0, xmm5, oword [eax+64], 0x9b
	vshufps ymm2, ymm0, yword [eax+1], 0xca
	vshufps ymm6, ymm1, yword [eax+64], 0x7b
	vshufps xmm0, xmm4, oword [eax+1], 0xc1
	vshufps xmm5, xmm2, oword [eax+64], 0xfd
	vshufps ymm5, ymm0, yword [eax+1], 0xb9
	vshufps ymm5, ymm1, yword [eax+64], 0xa7
	vshufps zmm5, zmm4, zword [eax+1], 0xc
	vshufps zmm7, zmm3, zword [eax+64], 0xd5
	vshufps xmm4, xmm5, [0x541], 0xdd
	vshufps ymm4, ymm5, [0xa78], 0x2d
	vshufps xmm4, xmm7, [0xc32], 0x11
	vshufps ymm1, ymm3, [0xa2c], 0x4
	vshufps zmm3, zmm7, [0xb65], 0x91
