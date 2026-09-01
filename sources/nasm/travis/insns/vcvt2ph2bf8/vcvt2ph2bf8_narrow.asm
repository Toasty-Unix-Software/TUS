	vcvt2ph2bf8 xmm1, xmm1, xmm4
	vcvt2ph2bf8 xmm5, xmm2, xmm6
	vcvt2ph2bf8 ymm7, ymm1, yword [0x14f]
	vcvt2ph2bf8 ymm2, ymm5, ymm6
	vcvt2ph2bf8 zmm7, zmm0, zmm2
	vcvt2ph2bf8 zmm6, zmm3, zmm2

%ifdef ERROR
	vcvt2ph2bf8 xmm10, xmm8, xmm12
	vcvt2ph2bf8 ymm14, ymm15, ymm9
	vcvt2ph2bf8 zmm11, zmm13, zmm9
	vcvt2ph2bf8 xmm23, xmm27, xmm17
	vcvt2ph2bf8 ymm16, ymm29, ymm16
	vcvt2ph2bf8 zmm30, zmm18, zmm18
%endif
