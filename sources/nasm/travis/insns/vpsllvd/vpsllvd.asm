default rel
	vpsllvd xmm7, xmm6, oword [0x2cd]
	vpsllvd xmm7, oword [0x2cd]
	vpsllvd xmm4, xmm1, xmm4
	vpsllvd ymm3, ymm4, yword [0x2a6]
	vpsllvd ymm3, yword [0x2a6]
	vpsllvd ymm2, ymm5, ymm6
	vpsllvd xmm3, xmm5, xmm4
	vpsllvd xmm3, xmm4
	vpsllvd xmm1, xmm0, oword [0xfde]
	vpsllvd ymm5, ymm4, ymm7
	vpsllvd ymm5, ymm7
	vpsllvd ymm3, ymm5, yword [0x448]
	vpsllvd xmm11, xmm15, xmm14
	vpsllvd ymm12, ymm15, ymm10
	vpsllvd xmm10, xmm9, xmm8
	vpsllvd ymm15, ymm14, ymm12
	vpsllvd xmm18, xmm21, xmm18
	vpsllvd ymm25, ymm25, ymm28
	vpsllvd xmm19, xmm19, xmm26
	vpsllvd ymm24, ymm30, ymm22
	vpsllvd xmm7{k6}, xmm5, xmm5
	vpsllvd ymm7{k3}, ymm0, yword [0x39d]
	vpsllvd zmm1{k4}, zmm5, zmm0
	vpsllvd xmm1{k7}{z}, xmm4, oword [0x4da]
	vpsllvd ymm4{k4}{z}, ymm5, yword [0xd7b]
	vpsllvd zmm5{k5}{z}, zmm1, zword [0xe7d]
	vpsllvd xmm6, xmm6, oword [eax+1]
	vpsllvd xmm5, xmm2, oword [eax+64]
	vpsllvd ymm6, ymm0, yword [eax+1]
	vpsllvd ymm6, ymm3, yword [eax+64]
	vpsllvd xmm4, xmm0, oword [eax+1]
	vpsllvd xmm4, xmm0, oword [eax+64]
	vpsllvd ymm7, ymm3, yword [eax+1]
	vpsllvd ymm3, ymm6, yword [eax+64]
	vpsllvd zmm1, zmm5, zword [eax+1]
	vpsllvd zmm0, zmm3, zword [eax+64]
	vpsllvd xmm0, xmm0, [0xc4c]
	vpsllvd ymm5, ymm5, [0x998]
	vpsllvd xmm7, xmm1, [0x387]
	vpsllvd ymm7, ymm1, [0x6f6]
	vpsllvd zmm3, zmm3, [0xb32]
