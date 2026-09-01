	vcmpge_osps xmm3, xmm3, xmm6
	vcmpge_osps xmm3, xmm6
	vcmpge_osps xmm2, xmm6, xmm4
	vcmpge_osps ymm7, ymm5, yword [0x584]
	vcmpge_osps ymm7, yword [0x584]
	vcmpge_osps ymm4, ymm2, ymm0
	vcmpge_osps k4, xmm7, xmm6
	vcmpge_osps k5, xmm1, xmm4
	vcmpge_osps k1, ymm3, ymm5
	vcmpge_osps k2, ymm0, ymm7

%ifdef ERROR
	vcmpge_osps xmm11, xmm15, xmm8
	vcmpge_osps ymm14, ymm9, ymm9
	vcmpge_osps k6, xmm11, xmm10
	vcmpge_osps k7, ymm11, ymm12
%endif
