	vcmpge_ospd xmm1, xmm4, xmm1
	vcmpge_ospd xmm1, xmm1
	vcmpge_ospd xmm0, xmm4, xmm4
	vcmpge_ospd ymm7, ymm3, ymm5
	vcmpge_ospd ymm7, ymm5
	vcmpge_ospd ymm7, ymm2, yword [0x30b]
	vcmpge_ospd k7, xmm4, xmm4
	vcmpge_ospd k3, xmm6, xmm6
	vcmpge_ospd k1, ymm0, ymm2
	vcmpge_ospd k4, ymm7, ymm1

%ifdef ERROR
	vcmpge_ospd xmm10, xmm11, xmm10
	vcmpge_ospd ymm11, ymm10, ymm13
	vcmpge_ospd k5, xmm13, xmm15
	vcmpge_ospd k6, ymm8, ymm8
%endif
