	vcmpngtss xmm4, xmm0, xmm3
	vcmpngtss xmm4, xmm3
	vcmpngtss xmm6, xmm3, xmm4
	vcmpngtss k2, xmm2, xmm2
	vcmpngtss k3, xmm6, xmm7

%ifdef ERROR
	vcmpngtss xmm15, xmm14, xmm9
	vcmpngtss k5, xmm14, xmm9
%endif
