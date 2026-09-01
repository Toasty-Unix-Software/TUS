default rel
	vcvtph2hf8s xmm6, xmm3
	vcvtph2hf8s xmm5, oword [0xa4e]
	vcvtph2hf8s xmm1, ymm7
	vcvtph2hf8s xmm1, ymm3
	vcvtph2hf8s ymm4, zmm7
	vcvtph2hf8s ymm1, zmm0
	vcvtph2hf8s xmm12, xmm8
	vcvtph2hf8s xmm9, ymm9
	vcvtph2hf8s ymm15, zmm8
	vcvtph2hf8s xmm28, xmm31
	vcvtph2hf8s xmm24, ymm18
	vcvtph2hf8s ymm20, zmm18
	vcvtph2hf8s xmm6{k4}, oword [0x8e1]
	vcvtph2hf8s xmm2{k7}, yword [0x8b9]
	vcvtph2hf8s ymm3{k5}, zmm6
	vcvtph2hf8s xmm6{k4}{z}, xmm6
	vcvtph2hf8s xmm4{k2}{z}, ymm7
	vcvtph2hf8s ymm4{k7}{z}, zmm7
	vcvtph2hf8s xmm6, oword [eax+1]
	vcvtph2hf8s xmm5, oword [eax+64]
	vcvtph2hf8s xmm6, yword [eax+1]
	vcvtph2hf8s xmm4, yword [eax+64]
	vcvtph2hf8s ymm5, zword [eax+1]
	vcvtph2hf8s ymm1, zword [eax+64]
	vcvtph2hf8s xmm4, [0x591]
	vcvtph2hf8s xmm5, [0xbd0]
	vcvtph2hf8s ymm1, [0x27c]
