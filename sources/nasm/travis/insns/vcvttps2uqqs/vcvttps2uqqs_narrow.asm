	vcvttps2uqqs xmm7, qword [0xf2a]
	vcvttps2uqqs xmm2, qword [0x799]
	vcvttps2uqqs ymm2, oword [0xbed]
	vcvttps2uqqs ymm2, xmm1
	vcvttps2uqqs zmm4, ymm7
	vcvttps2uqqs zmm2, ymm1

%ifdef ERROR
	vcvttps2uqqs xmm10, xmm12
	vcvttps2uqqs ymm15, xmm14
	vcvttps2uqqs zmm12, ymm11
	vcvttps2uqqs xmm23, xmm30
	vcvttps2uqqs ymm26, xmm21
	vcvttps2uqqs zmm24, ymm25
%endif
