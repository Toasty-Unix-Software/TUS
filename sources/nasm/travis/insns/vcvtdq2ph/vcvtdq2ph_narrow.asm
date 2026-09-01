	vcvtdq2ph xmm2, xmm5
	vcvtdq2ph xmm3, xmm2
	vcvtdq2ph xmm1, yword [0xc05]
	vcvtdq2ph xmm1, ymm3
	vcvtdq2ph ymm0, zmm2
	vcvtdq2ph ymm5, zmm6

%ifdef ERROR
	vcvtdq2ph xmm11, xmm12
	vcvtdq2ph xmm8, ymm13
	vcvtdq2ph ymm9, zmm8
	vcvtdq2ph xmm27, xmm31
	vcvtdq2ph xmm18, ymm20
	vcvtdq2ph ymm18, zmm19
%endif
