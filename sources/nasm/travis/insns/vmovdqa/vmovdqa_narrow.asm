	vmovdqa xmm3, xmm2
	vmovdqa xmm2, xmm0
	vmovdqa xmm2, xmm3
	vmovdqa xmm2, xmm6
	vmovdqa ymm1, ymm7
	vmovdqa ymm3, ymm1
	vmovdqa ymm1, ymm1
	vmovdqa yword [0x410], ymm2

%ifdef ERROR
	vmovdqa xmm8, xmm12
	vmovdqa xmm8, xmm14
	vmovdqa ymm10, ymm12
	vmovdqa ymm14, ymm9
%endif
