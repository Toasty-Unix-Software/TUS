	vmovshdup xmm5, xmm5
	vmovshdup xmm7, oword [0xb27]
	vmovshdup ymm0, ymm4
	vmovshdup ymm7, ymm7
	vmovshdup xmm4, xmm0
	vmovshdup xmm6, xmm0
	vmovshdup ymm2, yword [0xb46]
	vmovshdup ymm4, ymm0

%ifdef ERROR
	vmovshdup xmm11, xmm9
	vmovshdup ymm15, ymm11
	vmovshdup xmm13, xmm12
	vmovshdup ymm14, ymm12
	vmovshdup xmm25, xmm24
	vmovshdup ymm23, ymm28
	vmovshdup xmm18, xmm26
	vmovshdup ymm27, ymm25
%endif
