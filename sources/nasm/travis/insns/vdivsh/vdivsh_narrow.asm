	vdivsh xmm1, xmm3, xmm2
	vdivsh xmm1, xmm2
	vdivsh xmm2, xmm0, xmm1

%ifdef ERROR
	vdivsh xmm13, xmm8, xmm9
	vdivsh xmm24, xmm30, xmm18
%endif
