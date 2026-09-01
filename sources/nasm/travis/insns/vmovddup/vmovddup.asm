default rel
	vmovddup xmm2, xmm6
	vmovddup xmm5, qword [0x95d]
	vmovddup ymm6, ymm7
	vmovddup ymm0, yword [0x5b0]
	vmovddup xmm4, xmm3
	vmovddup xmm7, qword [0xd71]
	vmovddup ymm7, yword [0xfac]
	vmovddup ymm7, yword [0x703]
	vmovddup xmm10, xmm12
	vmovddup ymm14, ymm8
	vmovddup xmm13, xmm10
	vmovddup ymm15, ymm12
	vmovddup xmm23, xmm26
	vmovddup ymm31, ymm31
	vmovddup xmm21, xmm28
	vmovddup ymm25, ymm27
	vmovddup xmm1{k1}, xmm1
	vmovddup ymm3{k6}, ymm5
	vmovddup zmm6{k2}, zword [0xd11]
	vmovddup xmm3{k6}{z}, xmm1
	vmovddup ymm0{k6}{z}, yword [0xb50]
	vmovddup zmm7{k4}{z}, zmm1
	vmovddup xmm3, qword [eax+1]
	vmovddup xmm4, qword [eax+64]
	vmovddup ymm3, yword [eax+1]
	vmovddup ymm0, yword [eax+64]
	vmovddup xmm0, qword [eax+1]
	vmovddup xmm3, qword [eax+64]
	vmovddup ymm1, yword [eax+1]
	vmovddup ymm1, yword [eax+64]
	vmovddup zmm4, zword [eax+1]
	vmovddup zmm2, zword [eax+64]
	vmovddup xmm0, [0x304]
	vmovddup ymm3, [0x468]
	vmovddup xmm7, [0xc7d]
	vmovddup ymm6, [0xeab]
	vmovddup zmm1, [0x578]
