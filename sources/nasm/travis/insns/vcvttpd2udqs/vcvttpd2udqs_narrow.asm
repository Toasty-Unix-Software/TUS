	vcvttpd2udqs xmm7, xmm3
	vcvttpd2udqs xmm6, xmm0
	vcvttpd2udqs xmm7, ymm1
	vcvttpd2udqs xmm4, yword [0xf3e]
	vcvttpd2udqs ymm7, zmm0
	vcvttpd2udqs ymm2, zmm1

%ifdef ERROR
	vcvttpd2udqs xmm8, xmm8
	vcvttpd2udqs xmm13, ymm14
	vcvttpd2udqs ymm10, zmm9
	vcvttpd2udqs xmm22, xmm18
	vcvttpd2udqs xmm16, ymm23
	vcvttpd2udqs ymm16, zmm22
%endif
