	vcvttpd2uqqs xmm1, xmm2
	vcvttpd2uqqs xmm1, xmm1
	vcvttpd2uqqs ymm0, ymm2
	vcvttpd2uqqs ymm3, ymm5
	vcvttpd2uqqs zmm4, zmm6
	vcvttpd2uqqs zmm2, zmm3

%ifdef ERROR
	vcvttpd2uqqs xmm10, xmm11
	vcvttpd2uqqs ymm12, ymm12
	vcvttpd2uqqs zmm14, zmm15
	vcvttpd2uqqs xmm22, xmm16
	vcvttpd2uqqs ymm23, ymm27
	vcvttpd2uqqs zmm22, zmm28
%endif
