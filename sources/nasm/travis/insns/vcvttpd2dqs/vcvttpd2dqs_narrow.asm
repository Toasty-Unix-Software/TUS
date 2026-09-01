	vcvttpd2dqs xmm6, xmm1
	vcvttpd2dqs xmm1, xmm0
	vcvttpd2dqs xmm5, yword [0x240]
	vcvttpd2dqs xmm5, ymm6
	vcvttpd2dqs ymm5, zmm3
	vcvttpd2dqs ymm6, zmm4

%ifdef ERROR
	vcvttpd2dqs xmm14, xmm9
	vcvttpd2dqs xmm8, ymm13
	vcvttpd2dqs ymm12, zmm10
	vcvttpd2dqs xmm31, xmm23
	vcvttpd2dqs xmm21, ymm22
	vcvttpd2dqs ymm25, zmm31
%endif
