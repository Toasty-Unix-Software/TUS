default rel
	vmovdqu xmm0, xmm7
	vmovdqu xmm5, xmm0
	vmovdqu xmm2, xmm5
	vmovdqu xmm2, xmm6
	vmovdqu ymm1, yword [0xeaa]
	vmovdqu ymm6, yword [0xd0e]
	vmovdqu ymm0, ymm1
	vmovdqu yword [0xa49], ymm0
	vmovdqu xmm15, xmm11
	vmovdqu xmm12, xmm11
	vmovdqu ymm13, ymm12
	vmovdqu ymm14, ymm14
	vmovdqu xmm2, oword [eax+1]
	vmovdqu xmm3, oword [eax+64]
	vmovdqu oword [eax+1], xmm5
	vmovdqu oword [eax+64], xmm6
	vmovdqu ymm6, yword [eax+1]
	vmovdqu ymm7, yword [eax+64]
	vmovdqu yword [eax+1], ymm7
	vmovdqu yword [eax+64], ymm3
	vmovdqu xmm4, [0xa04]
	vmovdqu [0xa49], xmm1
	vmovdqu ymm1, [0x81e]
	vmovdqu [0x849], ymm5
