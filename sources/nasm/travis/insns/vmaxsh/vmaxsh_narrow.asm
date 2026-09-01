	vmaxsh xmm3, xmm1, xmm4
	vmaxsh xmm1, xmm6, word [0xcc2]
	vmaxsh xmm4, xmm5, xmm7
	vmaxsh xmm4, xmm7
	vmaxsh xmm6, xmm4, xmm1

%ifdef ERROR
	vmaxsh xmm14, xmm9, xmm11
	vmaxsh xmm14, xmm10, xmm14
	vmaxsh xmm17, xmm21, xmm18
	vmaxsh xmm30, xmm20, xmm19
%endif
