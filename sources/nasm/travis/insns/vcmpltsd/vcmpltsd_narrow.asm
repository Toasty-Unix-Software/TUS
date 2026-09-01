	vcmpltsd xmm0, xmm3, xmm6
	vcmpltsd xmm0, xmm6
	vcmpltsd xmm4, xmm3, xmm0
	vcmpltsd k7, xmm3, xmm5
	vcmpltsd k3, xmm7, xmm2

%ifdef ERROR
	vcmpltsd xmm13, xmm11, xmm9
	vcmpltsd k6, xmm11, xmm15
%endif
