default rel
	vpmulld xmm2, xmm6, xmm0
	vpmulld xmm2, xmm0
	vpmulld xmm7, xmm6, oword [0x3ea]
	vpmulld ymm6, ymm7, ymm1
	vpmulld ymm6, ymm1
	vpmulld ymm2, ymm5, ymm1
	vpmulld xmm3, xmm3, xmm4
	vpmulld xmm3, xmm4
	vpmulld xmm3, xmm1, oword [0xb78]
	vpmulld ymm0, ymm1, ymm0
	vpmulld ymm0, ymm0
	vpmulld ymm2, ymm2, ymm5
	vpmulld xmm11, xmm11, xmm8
	vpmulld ymm11, ymm10, ymm14
	vpmulld xmm15, xmm9, xmm12
	vpmulld ymm9, ymm15, ymm8
	vpmulld xmm20, xmm18, xmm17
	vpmulld ymm20, ymm24, ymm24
	vpmulld xmm22, xmm18, xmm26
	vpmulld ymm23, ymm22, ymm17
	vpmulld xmm4{k4}, xmm4, xmm2
	vpmulld ymm0{k3}, ymm6, ymm6
	vpmulld zmm4{k7}, zmm1, zmm4
	vpmulld xmm1{k2}{z}, xmm6, oword [0x93c]
	vpmulld ymm4{k5}{z}, ymm3, yword [0xe59]
	vpmulld zmm7{k3}{z}, zmm6, zmm2
	vpmulld xmm2, xmm1, oword [eax+1]
	vpmulld xmm6, xmm5, oword [eax+64]
	vpmulld ymm5, ymm7, yword [eax+1]
	vpmulld ymm0, ymm0, yword [eax+64]
	vpmulld xmm1, xmm3, oword [eax+1]
	vpmulld xmm1, xmm3, oword [eax+64]
	vpmulld ymm7, ymm3, yword [eax+1]
	vpmulld ymm3, ymm7, yword [eax+64]
	vpmulld zmm6, zmm0, zword [eax+1]
	vpmulld zmm1, zmm0, zword [eax+64]
	vpmulld xmm2, xmm3, [0x79c]
	vpmulld ymm0, ymm6, [0xad4]
	vpmulld xmm3, xmm2, [0xdd6]
	vpmulld ymm4, ymm1, [0x1a8]
	vpmulld zmm5, zmm2, [0x129]
