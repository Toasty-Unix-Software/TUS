	vcmpngt_uqsd xmm1, xmm4, xmm3
	vcmpngt_uqsd xmm1, xmm3
	vcmpngt_uqsd xmm1, xmm2, xmm5
	vcmpngt_uqsd k7, xmm5, xmm2
	vcmpngt_uqsd k3, xmm5, xmm1

%ifdef ERROR
	vcmpngt_uqsd xmm13, xmm8, xmm15
	vcmpngt_uqsd k5, xmm15, xmm13
%endif
