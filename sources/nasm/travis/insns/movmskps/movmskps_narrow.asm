	movmskps eax, xmm7
	movmskps eax, xmm3

%ifdef ERROR
	movmskps rbp, xmm0
	movmskps rdi, xmm2
	movmskps r10d, xmm15
	movmskps r8, xmm11
%endif
