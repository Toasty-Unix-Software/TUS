	vpmacsswd xmm5, xmm1, oword [0x75a], xmm7
	vpmacsswd xmm5, oword [0x75a], xmm7
	vpmacsswd xmm3, xmm1, xmm0, xmm1

%ifdef ERROR
	vpmacsswd xmm13, xmm8, xmm13, xmm9
%endif
