default rel
	vcvtph2bf8s xmm3, oword [0x8f1]
	vcvtph2bf8s xmm3, xmm2
	vcvtph2bf8s xmm3, ymm3
	vcvtph2bf8s xmm5, ymm4
	vcvtph2bf8s ymm1, zword [0x8cc]
	vcvtph2bf8s ymm7, zmm3
	vcvtph2bf8s xmm9, xmm10
	vcvtph2bf8s xmm8, ymm11
	vcvtph2bf8s ymm10, zmm9
	vcvtph2bf8s xmm16, xmm25
	vcvtph2bf8s xmm25, ymm29
	vcvtph2bf8s ymm23, zmm25
	vcvtph2bf8s xmm2{k1}, xmm6
	vcvtph2bf8s xmm1{k4}, ymm5
	vcvtph2bf8s ymm6{k1}, zmm1
	vcvtph2bf8s xmm2{k1}{z}, xmm7
	vcvtph2bf8s xmm5{k3}{z}, ymm2
	vcvtph2bf8s ymm3{k4}{z}, zmm5
	vcvtph2bf8s xmm2, oword [eax+1]
	vcvtph2bf8s xmm4, oword [eax+64]
	vcvtph2bf8s xmm7, yword [eax+1]
	vcvtph2bf8s xmm7, yword [eax+64]
	vcvtph2bf8s ymm3, zword [eax+1]
	vcvtph2bf8s ymm2, zword [eax+64]
	vcvtph2bf8s xmm2, [0x772]
	vcvtph2bf8s xmm5, [0x62b]
	vcvtph2bf8s ymm5, [0x2ee]
