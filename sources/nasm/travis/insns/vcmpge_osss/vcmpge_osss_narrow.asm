	vcmpge_osss xmm1, xmm3, xmm0
	vcmpge_osss xmm1, xmm0
	vcmpge_osss xmm1, xmm0, xmm1
	vcmpge_osss k1, xmm6, xmm4
	vcmpge_osss k3, xmm7, xmm0

%ifdef ERROR
	vcmpge_osss xmm13, xmm11, xmm12
	vcmpge_osss k3, xmm12, xmm15
%endif
