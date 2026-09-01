	vcmpfalse_ospd xmm5, xmm6, oword [0x554]
	vcmpfalse_ospd xmm5, oword [0x554]
	vcmpfalse_ospd xmm6, xmm3, xmm1
	vcmpfalse_ospd ymm7, ymm3, ymm3
	vcmpfalse_ospd ymm7, ymm3
	vcmpfalse_ospd ymm7, ymm6, ymm7
	vcmpfalse_ospd k2, xmm1, xmm4
	vcmpfalse_ospd k1, xmm3, xmm1
	vcmpfalse_ospd k1, ymm4, ymm1
	vcmpfalse_ospd k7, ymm7, ymm0

%ifdef ERROR
	vcmpfalse_ospd xmm12, xmm13, xmm9
	vcmpfalse_ospd ymm15, ymm12, ymm11
	vcmpfalse_ospd k1, xmm10, xmm10
	vcmpfalse_ospd k6, ymm12, ymm10
%endif
