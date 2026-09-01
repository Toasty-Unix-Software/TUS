default rel
	vcvtbiasph2bf8s xmm3, xmm4, xmm4
	vcvtbiasph2bf8s xmm7, xmm0, xmm4
	vcvtbiasph2bf8s xmm7, ymm7, yword [0xc5d]
	vcvtbiasph2bf8s xmm0, ymm6, ymm6
	vcvtbiasph2bf8s ymm2, zmm6, zmm4
	vcvtbiasph2bf8s ymm4, zmm3, zmm5
	vcvtbiasph2bf8s xmm12, xmm8, xmm8
	vcvtbiasph2bf8s xmm10, ymm15, ymm12
	vcvtbiasph2bf8s ymm9, zmm8, zmm12
	vcvtbiasph2bf8s xmm30, xmm29, xmm30
	vcvtbiasph2bf8s xmm20, ymm30, ymm30
	vcvtbiasph2bf8s ymm23, zmm28, zmm21
	vcvtbiasph2bf8s xmm5{k7}, xmm5, xmm2
	vcvtbiasph2bf8s xmm6{k7}, ymm5, ymm0
	vcvtbiasph2bf8s ymm6{k2}, zmm0, zmm5
	vcvtbiasph2bf8s xmm1{k3}{z}, xmm3, xmm7
	vcvtbiasph2bf8s xmm3{k5}{z}, ymm3, ymm4
	vcvtbiasph2bf8s ymm0{k7}{z}, zmm0, zmm1
	vcvtbiasph2bf8s xmm1, xmm6, oword [eax+1]
	vcvtbiasph2bf8s xmm5, xmm6, oword [eax+64]
	vcvtbiasph2bf8s xmm4, ymm7, yword [eax+1]
	vcvtbiasph2bf8s xmm1, ymm5, yword [eax+64]
	vcvtbiasph2bf8s ymm7, zmm7, zword [eax+1]
	vcvtbiasph2bf8s ymm5, zmm3, zword [eax+64]
	vcvtbiasph2bf8s xmm7, xmm0, [0x706]
	vcvtbiasph2bf8s xmm2, ymm1, [0x4ba]
	vcvtbiasph2bf8s ymm3, zmm0, [0x10d]
