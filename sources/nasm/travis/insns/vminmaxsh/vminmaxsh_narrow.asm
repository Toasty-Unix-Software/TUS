	vminmaxsh xmm2, xmm2, xmm6, 0x65
	vminmaxsh xmm4, xmm6, xmm1, 0x84

%ifdef ERROR
	vminmaxsh xmm12, xmm10, xmm14, 0xba
	vminmaxsh xmm22, xmm28, xmm23, 0xca
%endif
