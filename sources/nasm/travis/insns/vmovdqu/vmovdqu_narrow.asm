	vmovdqu xmm0, xmm7
	vmovdqu xmm5, xmm0
	vmovdqu xmm2, xmm5
	vmovdqu xmm2, xmm6
	vmovdqu ymm1, yword [0xeaa]
	vmovdqu ymm6, yword [0xd0e]
	vmovdqu ymm0, ymm1
	vmovdqu yword [0xa49], ymm0

%ifdef ERROR
	vmovdqu xmm15, xmm11
	vmovdqu xmm12, xmm11
	vmovdqu ymm13, ymm12
	vmovdqu ymm14, ymm14
%endif
