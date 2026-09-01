	vcvt2ph2hf8s xmm2, xmm4, xmm1
	vcvt2ph2hf8s xmm6, xmm4, xmm6
	vcvt2ph2hf8s ymm3, ymm1, ymm0
	vcvt2ph2hf8s ymm4, ymm2, ymm5
	vcvt2ph2hf8s zmm5, zmm3, zword [0x2f9]
	vcvt2ph2hf8s zmm6, zmm0, zmm3

%ifdef ERROR
	vcvt2ph2hf8s xmm8, xmm12, xmm14
	vcvt2ph2hf8s ymm9, ymm15, ymm11
	vcvt2ph2hf8s zmm8, zmm14, zmm13
	vcvt2ph2hf8s xmm24, xmm31, xmm29
	vcvt2ph2hf8s ymm27, ymm19, ymm25
	vcvt2ph2hf8s zmm18, zmm17, zmm24
%endif
