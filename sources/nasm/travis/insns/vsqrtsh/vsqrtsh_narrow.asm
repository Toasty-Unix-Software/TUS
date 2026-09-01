	vsqrtsh xmm6, xmm6, xmm4
	vsqrtsh xmm6, xmm4
	vsqrtsh xmm6, xmm2, word [0x669]

%ifdef ERROR
	vsqrtsh xmm9, xmm13, xmm9
	vsqrtsh xmm22, xmm29, xmm25
%endif
