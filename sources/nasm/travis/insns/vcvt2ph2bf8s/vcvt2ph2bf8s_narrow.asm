	vcvt2ph2bf8s xmm2, xmm1, xmm0
	vcvt2ph2bf8s xmm1, xmm2, xmm5
	vcvt2ph2bf8s ymm0, ymm1, yword [0x14a]
	vcvt2ph2bf8s ymm6, ymm6, ymm4
	vcvt2ph2bf8s zmm1, zmm2, zmm1
	vcvt2ph2bf8s zmm1, zmm7, zmm6

%ifdef ERROR
	vcvt2ph2bf8s xmm13, xmm12, xmm10
	vcvt2ph2bf8s ymm8, ymm13, ymm13
	vcvt2ph2bf8s zmm9, zmm13, zmm9
	vcvt2ph2bf8s xmm24, xmm27, xmm30
	vcvt2ph2bf8s ymm31, ymm16, ymm28
	vcvt2ph2bf8s zmm24, zmm27, zmm23
%endif
