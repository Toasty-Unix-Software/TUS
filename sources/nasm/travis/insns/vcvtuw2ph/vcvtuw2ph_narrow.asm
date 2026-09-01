	vcvtuw2ph xmm5, xmm4
	vcvtuw2ph xmm3, xmm1
	vcvtuw2ph ymm6, ymm1
	vcvtuw2ph ymm3, ymm3
	vcvtuw2ph zmm2, zmm6
	vcvtuw2ph zmm6, zmm3

%ifdef ERROR
	vcvtuw2ph xmm9, xmm8
	vcvtuw2ph ymm8, ymm9
	vcvtuw2ph zmm8, zmm8
	vcvtuw2ph xmm26, xmm30
	vcvtuw2ph ymm19, ymm25
	vcvtuw2ph zmm17, zmm21
%endif
