	movmskpd edx, xmm3
	movmskpd edi, xmm5

%ifdef ERROR
	movmskpd rbx, xmm6
	movmskpd rbp, xmm2
	movmskpd r8d, xmm13
	movmskpd r10, xmm11
%endif
