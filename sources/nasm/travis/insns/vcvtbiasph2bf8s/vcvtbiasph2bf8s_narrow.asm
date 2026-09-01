	vcvtbiasph2bf8s xmm3, xmm4, xmm4
	vcvtbiasph2bf8s xmm7, xmm0, xmm4
	vcvtbiasph2bf8s xmm7, ymm7, yword [0xc5d]
	vcvtbiasph2bf8s xmm0, ymm6, ymm6
	vcvtbiasph2bf8s ymm2, zmm6, zmm4
	vcvtbiasph2bf8s ymm4, zmm3, zmm5

%ifdef ERROR
	vcvtbiasph2bf8s xmm12, xmm8, xmm8
	vcvtbiasph2bf8s xmm10, ymm15, ymm12
	vcvtbiasph2bf8s ymm9, zmm8, zmm12
	vcvtbiasph2bf8s xmm30, xmm29, xmm30
	vcvtbiasph2bf8s xmm20, ymm30, ymm30
	vcvtbiasph2bf8s ymm23, zmm28, zmm21
%endif
