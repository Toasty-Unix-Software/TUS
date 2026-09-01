	vcmpord_ssd xmm3, xmm1, xmm6
	vcmpord_ssd xmm3, xmm6
	vcmpord_ssd xmm6, xmm4, xmm0
	vcmpord_ssd k7, xmm2, xmm6
	vcmpord_ssd k2, xmm0, xmm0

%ifdef ERROR
	vcmpord_ssd xmm13, xmm8, xmm15
	vcmpord_ssd k2, xmm11, xmm9
%endif
