	vcmpfalse_osps xmm1, xmm0, oword [0x50e]
	vcmpfalse_osps xmm1, oword [0x50e]
	vcmpfalse_osps xmm7, xmm7, xmm3
	vcmpfalse_osps ymm0, ymm5, yword [0xc34]
	vcmpfalse_osps ymm0, yword [0xc34]
	vcmpfalse_osps ymm2, ymm2, ymm0
	vcmpfalse_osps k3, xmm0, xmm7
	vcmpfalse_osps k3, xmm1, xmm5
	vcmpfalse_osps k7, ymm4, ymm1
	vcmpfalse_osps k6, ymm2, ymm0

%ifdef ERROR
	vcmpfalse_osps xmm13, xmm10, xmm10
	vcmpfalse_osps ymm11, ymm10, ymm12
	vcmpfalse_osps k5, xmm15, xmm15
	vcmpfalse_osps k5, ymm11, ymm8
%endif
