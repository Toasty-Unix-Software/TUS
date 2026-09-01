	packsswb mm5, mm2
	packsswb mm1, mm0
	packsswb xmm7, oword [0xd0c]
	packsswb xmm5, xmm1

%ifdef ERROR
	packsswb xmm8, xmm13
%endif
