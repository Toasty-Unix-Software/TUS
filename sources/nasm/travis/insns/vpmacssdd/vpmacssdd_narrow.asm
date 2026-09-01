	vpmacssdd xmm5, xmm0, oword [0xaee], xmm6
	vpmacssdd xmm5, oword [0xaee], xmm6
	vpmacssdd xmm3, xmm5, oword [0xf99], xmm0

%ifdef ERROR
	vpmacssdd xmm11, xmm11, xmm13, xmm14
%endif
