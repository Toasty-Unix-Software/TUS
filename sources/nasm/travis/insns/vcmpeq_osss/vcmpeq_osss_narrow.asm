	vcmpeq_osss xmm6, xmm0, xmm3
	vcmpeq_osss xmm6, xmm3
	vcmpeq_osss xmm6, xmm7, xmm1
	vcmpeq_osss k2, xmm6, xmm4
	vcmpeq_osss k2, xmm1, xmm4

%ifdef ERROR
	vcmpeq_osss xmm10, xmm13, xmm13
	vcmpeq_osss k6, xmm15, xmm9
%endif
