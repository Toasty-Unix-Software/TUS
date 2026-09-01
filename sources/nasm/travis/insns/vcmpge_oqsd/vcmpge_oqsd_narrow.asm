	vcmpge_oqsd xmm7, xmm6, xmm7
	vcmpge_oqsd xmm7, xmm7
	vcmpge_oqsd xmm2, xmm7, xmm1
	vcmpge_oqsd k3, xmm5, xmm5
	vcmpge_oqsd k4, xmm2, xmm0

%ifdef ERROR
	vcmpge_oqsd xmm8, xmm11, xmm13
	vcmpge_oqsd k2, xmm15, xmm8
%endif
