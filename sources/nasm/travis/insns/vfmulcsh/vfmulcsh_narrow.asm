	vfmulcsh xmm2, xmm3, xmm1
	vfmulcsh xmm2, xmm1
	vfmulcsh xmm2, xmm1, xmm4

%ifdef ERROR
	vfmulcsh xmm9, xmm13, xmm11
	vfmulcsh xmm21, xmm24, xmm28
%endif
