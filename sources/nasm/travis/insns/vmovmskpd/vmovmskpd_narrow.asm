	vmovmskpd ebp, xmm3
	vmovmskpd esi, xmm7
	vmovmskpd ebx, ymm0
	vmovmskpd edi, ymm3

%ifdef ERROR
	vmovmskpd rbp, xmm0
	vmovmskpd rbp, xmm2
	vmovmskpd rbp, ymm0
	vmovmskpd rbx, ymm1
	vmovmskpd r10, xmm11
	vmovmskpd r14d, xmm13
	vmovmskpd r9, ymm15
	vmovmskpd r9d, ymm13
%endif
