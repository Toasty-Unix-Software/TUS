	aeskeygenassist xmm4, xmm0, 0x14
	aeskeygenassist xmm5, xmm3, 0xfd

%ifdef ERROR
	aeskeygenassist xmm14, xmm11, 0x2d
%endif
