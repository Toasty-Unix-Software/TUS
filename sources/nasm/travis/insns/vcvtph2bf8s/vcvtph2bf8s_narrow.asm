	vcvtph2bf8s xmm3, oword [0x8f1]
	vcvtph2bf8s xmm3, xmm2
	vcvtph2bf8s xmm3, ymm3
	vcvtph2bf8s xmm5, ymm4
	vcvtph2bf8s ymm1, zword [0x8cc]
	vcvtph2bf8s ymm7, zmm3

%ifdef ERROR
	vcvtph2bf8s xmm9, xmm10
	vcvtph2bf8s xmm8, ymm11
	vcvtph2bf8s ymm10, zmm9
	vcvtph2bf8s xmm16, xmm25
	vcvtph2bf8s xmm25, ymm29
	vcvtph2bf8s ymm23, zmm25
%endif
