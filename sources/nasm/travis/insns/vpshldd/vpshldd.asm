default rel
	vpshldd xmm1, xmm7, xmm5, 0x34
	vpshldd xmm1, xmm5, 0x34
	vpshldd xmm3, xmm5, xmm0, 0xc6
	vpshldd ymm3, ymm4, yword [0xcd6], 0xca
	vpshldd ymm3, yword [0xcd6], 0xca
	vpshldd ymm2, ymm5, ymm3, 0x53
	vpshldd zmm4, zmm3, zmm6, 0x12
	vpshldd zmm4, zmm6, 0x12
	vpshldd zmm3, zmm1, zmm7, 0xc5
	vpshldd xmm15, xmm14, xmm11, 0xae
	vpshldd ymm12, ymm12, ymm11, 0x90
	vpshldd zmm10, zmm9, zmm14, 0x62
	vpshldd xmm23, xmm20, xmm23, 0xc3
	vpshldd ymm28, ymm23, ymm28, 0x2d
	vpshldd zmm22, zmm31, zmm22, 0x5d
	vpshldd xmm3{k6}, xmm0, xmm3, 0xa8
	vpshldd ymm0{k3}, ymm6, ymm2, 0x59
	vpshldd zmm7{k2}, zmm7, zmm0, 0x3d
	vpshldd xmm2{k1}{z}, xmm0, xmm4, 0xf9
	vpshldd ymm3{k6}{z}, ymm5, yword [0xc3b], 0xd8
	vpshldd zmm7{k4}{z}, zmm1, zword [0x44b], 0xe2
	vpshldd xmm3, xmm7, oword [eax+1], 0x8
	vpshldd xmm0, xmm1, oword [eax+64], 0x49
	vpshldd ymm2, ymm4, yword [eax+1], 0x8f
	vpshldd ymm3, ymm5, yword [eax+64], 0x95
	vpshldd zmm6, zmm4, zword [eax+1], 0xe
	vpshldd zmm3, zmm4, zword [eax+64], 0x4c
	vpshldd xmm7, xmm2, [0x7a5], 0xd8
	vpshldd ymm5, ymm0, [0x424], 0xb9
	vpshldd zmm0, zmm7, [0x7e2], 0xa8
