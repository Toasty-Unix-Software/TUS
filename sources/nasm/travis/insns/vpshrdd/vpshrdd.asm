default rel
	vpshrdd xmm7, xmm3, xmm3, 0x96
	vpshrdd xmm7, xmm3, 0x96
	vpshrdd xmm1, xmm6, oword [0xabb], 0x66
	vpshrdd ymm4, ymm5, ymm0, 0x46
	vpshrdd ymm4, ymm0, 0x46
	vpshrdd ymm2, ymm3, ymm3, 0xb3
	vpshrdd zmm5, zmm7, zmm6, 0xa5
	vpshrdd zmm5, zmm6, 0xa5
	vpshrdd zmm5, zmm1, zword [0xfb2], 0xda
	vpshrdd xmm13, xmm11, xmm13, 0xd6
	vpshrdd ymm10, ymm12, ymm8, 0x54
	vpshrdd zmm11, zmm11, zmm13, 0xa2
	vpshrdd xmm25, xmm27, xmm30, 0x2c
	vpshrdd ymm20, ymm19, ymm31, 0x9c
	vpshrdd zmm18, zmm31, zmm20, 0xfe
	vpshrdd xmm5{k7}, xmm6, oword [0xce8], 0x67
	vpshrdd ymm0{k4}, ymm4, ymm7, 0xd1
	vpshrdd zmm6{k5}, zmm2, zmm1, 0xa7
	vpshrdd xmm5{k7}{z}, xmm5, oword [0x8d3], 0x1c
	vpshrdd ymm0{k3}{z}, ymm5, ymm7, 0xc1
	vpshrdd zmm2{k5}{z}, zmm6, zmm0, 0xb0
	vpshrdd xmm5, xmm7, oword [eax+1], 0xc4
	vpshrdd xmm2, xmm0, oword [eax+64], 0x3
	vpshrdd ymm5, ymm4, yword [eax+1], 0xd4
	vpshrdd ymm6, ymm0, yword [eax+64], 0xad
	vpshrdd zmm4, zmm1, zword [eax+1], 0x57
	vpshrdd zmm2, zmm6, zword [eax+64], 0x40
	vpshrdd xmm1, xmm3, [0x718], 0x5a
	vpshrdd ymm1, ymm3, [0xa9b], 0x1e
	vpshrdd zmm3, zmm0, [0x41d], 0x80
