default rel
	vmovshdup xmm5, xmm5
	vmovshdup xmm7, oword [0xb27]
	vmovshdup ymm0, ymm4
	vmovshdup ymm7, ymm7
	vmovshdup xmm4, xmm0
	vmovshdup xmm6, xmm0
	vmovshdup ymm2, yword [0xb46]
	vmovshdup ymm4, ymm0
	vmovshdup xmm11, xmm9
	vmovshdup ymm15, ymm11
	vmovshdup xmm13, xmm12
	vmovshdup ymm14, ymm12
	vmovshdup xmm25, xmm24
	vmovshdup ymm23, ymm28
	vmovshdup xmm18, xmm26
	vmovshdup ymm27, ymm25
	vmovshdup xmm5{k2}, oword [0xa0d]
	vmovshdup ymm4{k6}, ymm3
	vmovshdup zmm5{k3}, zword [0x4ae]
	vmovshdup xmm2{k7}{z}, xmm6
	vmovshdup ymm5{k6}{z}, ymm5
	vmovshdup zmm7{k1}{z}, zmm7
	vmovshdup xmm2, oword [eax+1]
	vmovshdup xmm1, oword [eax+64]
	vmovshdup ymm7, yword [eax+1]
	vmovshdup ymm4, yword [eax+64]
	vmovshdup xmm3, oword [eax+1]
	vmovshdup xmm4, oword [eax+64]
	vmovshdup ymm4, yword [eax+1]
	vmovshdup ymm6, yword [eax+64]
	vmovshdup zmm1, zword [eax+1]
	vmovshdup zmm2, zword [eax+64]
	vmovshdup xmm0, [0x9de]
	vmovshdup ymm3, [0xdd3]
	vmovshdup xmm0, [0xed4]
	vmovshdup ymm3, [0xb96]
	vmovshdup zmm7, [0xd88]
