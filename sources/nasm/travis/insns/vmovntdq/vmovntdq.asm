default rel
	vmovntdq oword [0xec5], xmm0
	vmovntdq oword [0x7da], xmm7
	vmovntdq yword [0x168], ymm7
	vmovntdq yword [0x5eb], ymm4
	vmovntdq zword [0xa11], zmm7
	vmovntdq zword [0x808], zmm0
	vmovntdq oword [0x6c5], xmm9
	vmovntdq yword [0x975], ymm12
	vmovntdq zword [0x942], zmm15
	vmovntdq oword [0xbb4], xmm24
	vmovntdq yword [0xfc1], ymm23
	vmovntdq zword [0x5ff], zmm28
	vmovntdq oword [eax+1], xmm3
	vmovntdq oword [eax+64], xmm5
	vmovntdq yword [eax+1], ymm2
	vmovntdq yword [eax+64], ymm6
	vmovntdq zword [eax+1], zmm2
	vmovntdq zword [eax+64], zmm0
	vmovntdq [0x4f0], xmm3
	vmovntdq [0x5da], ymm7
	vmovntdq [0xcd6], zmm0
