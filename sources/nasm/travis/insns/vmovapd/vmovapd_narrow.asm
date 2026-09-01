	vmovapd xmm6, oword [0xf62]
	vmovapd xmm5, xmm4
	vmovapd oword [0x18c], xmm3
	vmovapd xmm2, xmm6
	vmovapd ymm3, ymm7
	vmovapd ymm1, ymm1
	vmovapd ymm0, ymm7
	vmovapd ymm7, ymm6

%ifdef ERROR
	vmovapd xmm15, xmm8
	vmovapd xmm14, xmm9
	vmovapd ymm14, ymm9
	vmovapd ymm15, ymm11
	vmovapd xmm16, xmm22
	vmovapd xmm31, xmm26
	vmovapd ymm30, ymm18
	vmovapd ymm28, ymm17
%endif
