	vsubsh xmm3, xmm6, xmm3
	vsubsh xmm3, xmm3
	vsubsh xmm3, xmm2, word [0xcb7]

%ifdef ERROR
	vsubsh xmm12, xmm13, xmm11
	vsubsh xmm30, xmm28, xmm17
%endif
