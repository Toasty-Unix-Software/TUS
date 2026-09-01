	vaddsh xmm7, xmm5, xmm4
	vaddsh xmm7, xmm4
	vaddsh xmm3, xmm1, word [0xaba]

%ifdef ERROR
	vaddsh xmm11, xmm14, xmm13
	vaddsh xmm21, xmm30, xmm17
%endif
