default rel
	vmovdqa xmm3, xmm2
	vmovdqa xmm2, xmm0
	vmovdqa xmm2, xmm3
	vmovdqa xmm2, xmm6
	vmovdqa ymm1, ymm7
	vmovdqa ymm3, ymm1
	vmovdqa ymm1, ymm1
	vmovdqa yword [0x410], ymm2
	vmovdqa xmm8, xmm12
	vmovdqa xmm8, xmm14
	vmovdqa ymm10, ymm12
	vmovdqa ymm14, ymm9
	vmovdqa xmm3, oword [eax+1]
	vmovdqa xmm0, oword [eax+64]
	vmovdqa oword [eax+1], xmm3
	vmovdqa oword [eax+64], xmm1
	vmovdqa ymm7, yword [eax+1]
	vmovdqa ymm1, yword [eax+64]
	vmovdqa yword [eax+1], ymm6
	vmovdqa yword [eax+64], ymm4
	vmovdqa xmm2, [0xc23]
	vmovdqa [0x500], xmm1
	vmovdqa ymm5, [0x528]
	vmovdqa [0x4aa], ymm3
