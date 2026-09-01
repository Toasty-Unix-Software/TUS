	vcvttps2qqs xmm0, xmm0
	vcvttps2qqs xmm3, xmm6
	vcvttps2qqs ymm6, xmm3
	vcvttps2qqs ymm2, xmm6
	vcvttps2qqs zmm3, ymm7
	vcvttps2qqs zmm2, ymm2

%ifdef ERROR
	vcvttps2qqs xmm9, xmm9
	vcvttps2qqs ymm8, xmm12
	vcvttps2qqs zmm10, ymm14
	vcvttps2qqs xmm22, xmm18
	vcvttps2qqs ymm23, xmm16
	vcvttps2qqs zmm24, ymm29
%endif
