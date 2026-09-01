	vcmple_ossd xmm3, xmm2, xmm2
	vcmple_ossd xmm3, xmm2
	vcmple_ossd xmm7, xmm1, xmm3
	vcmple_ossd k6, xmm2, xmm7
	vcmple_ossd k5, xmm4, xmm0

%ifdef ERROR
	vcmple_ossd xmm10, xmm13, xmm8
	vcmple_ossd k2, xmm9, xmm13
%endif
