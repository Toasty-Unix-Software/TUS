	vcvttps2udqs xmm7, xmm0
	vcvttps2udqs xmm7, xmm3
	vcvttps2udqs ymm1, ymm3
	vcvttps2udqs ymm5, ymm4
	vcvttps2udqs zmm0, zmm5
	vcvttps2udqs zmm6, zmm3

%ifdef ERROR
	vcvttps2udqs xmm9, xmm8
	vcvttps2udqs ymm8, ymm12
	vcvttps2udqs zmm12, zmm9
	vcvttps2udqs xmm19, xmm17
	vcvttps2udqs ymm31, ymm26
	vcvttps2udqs zmm27, zmm20
%endif
