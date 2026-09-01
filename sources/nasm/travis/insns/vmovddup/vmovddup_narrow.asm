	vmovddup xmm2, xmm6
	vmovddup xmm5, qword [0x95d]
	vmovddup ymm6, ymm7
	vmovddup ymm0, yword [0x5b0]
	vmovddup xmm4, xmm3
	vmovddup xmm7, qword [0xd71]
	vmovddup ymm7, yword [0xfac]
	vmovddup ymm7, yword [0x703]

%ifdef ERROR
	vmovddup xmm10, xmm12
	vmovddup ymm14, ymm8
	vmovddup xmm13, xmm10
	vmovddup ymm15, ymm12
	vmovddup xmm23, xmm26
	vmovddup ymm31, ymm31
	vmovddup xmm21, xmm28
	vmovddup ymm25, ymm27
%endif
