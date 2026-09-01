	vmaxss xmm7, xmm6, xmm4
	vmaxss xmm7, xmm4
	vmaxss xmm0, xmm5, xmm5
	vmaxss xmm7, xmm6, xmm3
	vmaxss xmm7, xmm3
	vmaxss xmm0, xmm7, xmm0

%ifdef ERROR
	vmaxss xmm8, xmm11, xmm14
	vmaxss xmm13, xmm13, xmm12
	vmaxss xmm22, xmm27, xmm17
	vmaxss xmm16, xmm24, xmm26
%endif
