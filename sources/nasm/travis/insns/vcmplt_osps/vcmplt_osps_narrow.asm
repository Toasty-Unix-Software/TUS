	vcmplt_osps xmm2, xmm2, xmm0
	vcmplt_osps xmm2, xmm0
	vcmplt_osps xmm5, xmm0, xmm3
	vcmplt_osps ymm5, ymm5, ymm7
	vcmplt_osps ymm5, ymm7
	vcmplt_osps ymm1, ymm6, ymm1
	vcmplt_osps k7, xmm1, oword [0x5f8]
	vcmplt_osps k6, xmm7, xmm3
	vcmplt_osps k1, ymm3, ymm1
	vcmplt_osps k3, ymm4, yword [0x7a3]

%ifdef ERROR
	vcmplt_osps xmm13, xmm12, xmm11
	vcmplt_osps ymm9, ymm11, ymm10
	vcmplt_osps k2, xmm12, xmm11
	vcmplt_osps k6, ymm13, ymm8
%endif
