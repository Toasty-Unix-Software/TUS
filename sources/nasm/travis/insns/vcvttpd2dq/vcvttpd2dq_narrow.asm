	vcvttpd2dq xmm1, xmm1
	vcvttpd2dq xmm6, xmm5
	vcvttpd2dq xmm3, oword [0x961]
	vcvttpd2dq xmm1, oword [0x283]
	vcvttpd2dq xmm4, ymm6
	vcvttpd2dq xmm4, ymm4
	vcvttpd2dq xmm7, yword [0x7f1]
	vcvttpd2dq xmm6, yword [0xc06]

%ifdef ERROR
	vcvttpd2dq xmm11, xmm14
	vcvttpd2dq xmm14, oword [0xe11]
	vcvttpd2dq xmm10, ymm11
	vcvttpd2dq xmm13, yword [0x8b2]
	vcvttpd2dq xmm20, xmm18
	vcvttpd2dq xmm23, oword [0x63a]
	vcvttpd2dq xmm26, ymm28
	vcvttpd2dq xmm20, yword [0xf6c]
%endif
