	vcmpneq_osss xmm7, xmm1, xmm6
	vcmpneq_osss xmm7, xmm6
	vcmpneq_osss xmm0, xmm3, xmm3
	vcmpneq_osss k3, xmm0, xmm1
	vcmpneq_osss k7, xmm0, xmm1

%ifdef ERROR
	vcmpneq_osss xmm9, xmm9, xmm15
	vcmpneq_osss k2, xmm9, xmm14
%endif
