	vpmadcswd xmm5, xmm4, xmm2, xmm4
	vpmadcswd xmm5, xmm2, xmm4
	vpmadcswd xmm7, xmm0, oword [0x2f0], xmm6

%ifdef ERROR
	vpmadcswd xmm14, xmm10, xmm10, xmm10
%endif
