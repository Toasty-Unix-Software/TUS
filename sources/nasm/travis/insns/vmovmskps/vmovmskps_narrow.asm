	vmovmskps edx, xmm1
	vmovmskps ebx, xmm5
	vmovmskps eax, ymm1
	vmovmskps ecx, ymm7

%ifdef ERROR
	vmovmskps rbx, xmm1
	vmovmskps rax, xmm2
	vmovmskps rcx, ymm6
	vmovmskps rsi, ymm3
	vmovmskps r13, xmm13
	vmovmskps r12d, xmm14
	vmovmskps r12, ymm14
	vmovmskps r8d, ymm12
%endif
