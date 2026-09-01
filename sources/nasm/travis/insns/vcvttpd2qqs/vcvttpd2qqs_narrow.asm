	vcvttpd2qqs xmm3, oword [0xa5c]
	vcvttpd2qqs xmm7, xmm1
	vcvttpd2qqs ymm1, ymm1
	vcvttpd2qqs ymm1, ymm5
	vcvttpd2qqs zmm2, zword [0x81a]
	vcvttpd2qqs zmm5, zmm1

%ifdef ERROR
	vcvttpd2qqs xmm11, xmm12
	vcvttpd2qqs ymm10, ymm8
	vcvttpd2qqs zmm10, zmm14
	vcvttpd2qqs xmm23, xmm30
	vcvttpd2qqs ymm17, ymm21
	vcvttpd2qqs zmm20, zmm26
%endif
