default rel
	vcvtbiasph2hf8s xmm5, xmm6, xmm4
	vcvtbiasph2hf8s xmm3, xmm0, xmm7
	vcvtbiasph2hf8s xmm2, ymm0, yword [0x350]
	vcvtbiasph2hf8s xmm6, ymm5, ymm1
	vcvtbiasph2hf8s ymm0, zmm0, zmm7
	vcvtbiasph2hf8s ymm7, zmm3, zmm6
	vcvtbiasph2hf8s xmm12, xmm9, xmm12
	vcvtbiasph2hf8s xmm8, ymm13, ymm8
	vcvtbiasph2hf8s ymm8, zmm10, zmm10
	vcvtbiasph2hf8s xmm26, xmm17, xmm24
	vcvtbiasph2hf8s xmm23, ymm22, ymm27
	vcvtbiasph2hf8s ymm18, zmm22, zmm31
	vcvtbiasph2hf8s xmm0{k3}, xmm4, xmm1
	vcvtbiasph2hf8s xmm4{k7}, ymm7, ymm1
	vcvtbiasph2hf8s ymm2{k4}, zmm3, zmm6
	vcvtbiasph2hf8s xmm3{k7}{z}, xmm7, xmm5
	vcvtbiasph2hf8s xmm1{k4}{z}, ymm7, ymm5
	vcvtbiasph2hf8s ymm0{k5}{z}, zmm3, zword [0x2cf]
	vcvtbiasph2hf8s xmm1, xmm5, oword [eax+1]
	vcvtbiasph2hf8s xmm1, xmm5, oword [eax+64]
	vcvtbiasph2hf8s xmm1, ymm2, yword [eax+1]
	vcvtbiasph2hf8s xmm3, ymm1, yword [eax+64]
	vcvtbiasph2hf8s ymm2, zmm5, zword [eax+1]
	vcvtbiasph2hf8s ymm4, zmm3, zword [eax+64]
	vcvtbiasph2hf8s xmm1, xmm2, [0xea9]
	vcvtbiasph2hf8s xmm6, ymm6, [0x80b]
	vcvtbiasph2hf8s ymm1, zmm5, [0x4c8]
