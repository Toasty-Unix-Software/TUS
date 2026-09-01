	movdqa xmm6, xmm2
	movdqa xmm2, xmm5
	movdqa oword [0x4a9], xmm7
	movdqa oword [0x56b], xmm0

%ifdef ERROR
	movdqa xmm8, xmm14
	movdqa xmm11, xmm11
%endif
