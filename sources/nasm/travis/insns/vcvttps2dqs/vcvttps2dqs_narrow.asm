	vcvttps2dqs xmm7, xmm7
	vcvttps2dqs xmm3, xmm6
	vcvttps2dqs ymm6, ymm1
	vcvttps2dqs ymm6, yword [0xfd9]
	vcvttps2dqs zmm3, zmm3
	vcvttps2dqs zmm0, zmm4

%ifdef ERROR
	vcvttps2dqs xmm11, xmm13
	vcvttps2dqs ymm13, ymm8
	vcvttps2dqs zmm9, zmm10
	vcvttps2dqs xmm23, xmm18
	vcvttps2dqs ymm20, ymm26
	vcvttps2dqs zmm19, zmm19
%endif
