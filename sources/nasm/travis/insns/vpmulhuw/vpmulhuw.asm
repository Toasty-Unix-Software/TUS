default rel
	vpmulhuw xmm5, xmm3, oword [0xe5c]
	vpmulhuw xmm5, oword [0xe5c]
	vpmulhuw xmm3, xmm6, oword [0xa53]
	vpmulhuw ymm6, ymm2, yword [0x2a2]
	vpmulhuw ymm6, yword [0x2a2]
	vpmulhuw ymm5, ymm7, ymm5
	vpmulhuw xmm0, xmm0, xmm6
	vpmulhuw xmm0, xmm6
	vpmulhuw xmm1, xmm1, oword [0x786]
	vpmulhuw ymm6, ymm0, yword [0x341]
	vpmulhuw ymm6, yword [0x341]
	vpmulhuw ymm7, ymm2, yword [0x12e]
	vpmulhuw xmm9, xmm11, xmm8
	vpmulhuw ymm14, ymm9, ymm12
	vpmulhuw xmm9, xmm11, xmm15
	vpmulhuw ymm12, ymm14, ymm12
	vpmulhuw xmm19, xmm16, xmm30
	vpmulhuw ymm24, ymm16, ymm21
	vpmulhuw xmm31, xmm27, xmm28
	vpmulhuw ymm31, ymm16, ymm30
	vpmulhuw xmm7{k7}, xmm2, xmm7
	vpmulhuw ymm2{k2}, ymm2, ymm5
	vpmulhuw zmm4{k3}, zmm3, zmm6
	vpmulhuw xmm1{k7}{z}, xmm6, xmm4
	vpmulhuw ymm4{k7}{z}, ymm4, ymm1
	vpmulhuw zmm0{k7}{z}, zmm3, zmm3
	vpmulhuw xmm2, xmm6, oword [eax+1]
	vpmulhuw xmm3, xmm1, oword [eax+64]
	vpmulhuw ymm6, ymm4, yword [eax+1]
	vpmulhuw ymm6, ymm3, yword [eax+64]
	vpmulhuw xmm5, xmm7, oword [eax+1]
	vpmulhuw xmm2, xmm1, oword [eax+64]
	vpmulhuw ymm1, ymm0, yword [eax+1]
	vpmulhuw ymm2, ymm4, yword [eax+64]
	vpmulhuw zmm2, zmm1, zword [eax+1]
	vpmulhuw zmm6, zmm1, zword [eax+64]
	vpmulhuw xmm1, xmm0, [0x3bf]
	vpmulhuw ymm4, ymm5, [0x151]
	vpmulhuw xmm7, xmm1, [0x149]
	vpmulhuw ymm3, ymm4, [0x9e9]
	vpmulhuw zmm2, zmm6, [0x389]
