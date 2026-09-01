	vcmple_osss xmm0, xmm1, xmm6
	vcmple_osss xmm0, xmm6
	vcmple_osss xmm2, xmm0, xmm4
	vcmple_osss k2, xmm4, xmm0
	vcmple_osss k3, xmm0, xmm6

%ifdef ERROR
	vcmple_osss xmm12, xmm8, xmm8
	vcmple_osss k5, xmm13, xmm13
%endif
