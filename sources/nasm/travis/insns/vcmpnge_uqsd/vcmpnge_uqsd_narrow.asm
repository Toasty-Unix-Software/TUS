	vcmpnge_uqsd xmm7, xmm0, xmm2
	vcmpnge_uqsd xmm7, xmm2
	vcmpnge_uqsd xmm7, xmm0, xmm3
	vcmpnge_uqsd k3, xmm1, xmm3
	vcmpnge_uqsd k5, xmm4, xmm1

%ifdef ERROR
	vcmpnge_uqsd xmm13, xmm8, xmm13
	vcmpnge_uqsd k5, xmm8, xmm11
%endif
