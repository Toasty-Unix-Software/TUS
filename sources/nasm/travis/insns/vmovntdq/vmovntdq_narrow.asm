	vmovntdq oword [0xec5], xmm0
	vmovntdq oword [0x7da], xmm7
	vmovntdq yword [0x168], ymm7
	vmovntdq yword [0x5eb], ymm4
	vmovntdq zword [0xa11], zmm7
	vmovntdq zword [0x808], zmm0

%ifdef ERROR
	vmovntdq oword [0x6c5], xmm9
	vmovntdq yword [0x975], ymm12
	vmovntdq zword [0x942], zmm15
	vmovntdq oword [0xbb4], xmm24
	vmovntdq yword [0xfc1], ymm23
	vmovntdq zword [0x5ff], zmm28
%endif
