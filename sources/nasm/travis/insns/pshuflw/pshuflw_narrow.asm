	pshuflw xmm4, xmm7, 0x3
	pshuflw xmm2, xmm4, 0x26
	pshuflw xmm5, [0xdda], 0x9
	pshuflw xmm6, [0xf47], 0x63

%ifdef ERROR
	pshuflw xmm9, xmm15, 0x3f
	pshuflw xmm9, [0xe2a], 0x7
%endif
