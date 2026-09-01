default rel
	vpminuw xmm7, xmm3, oword [0xeba]
	vpminuw xmm7, oword [0xeba]
	vpminuw xmm1, xmm4, xmm1
	vpminuw ymm2, ymm4, ymm1
	vpminuw ymm2, ymm1
	vpminuw ymm3, ymm7, ymm7
	vpminuw xmm5, xmm1, xmm4
	vpminuw xmm5, xmm4
	vpminuw xmm0, xmm1, xmm0
	vpminuw ymm7, ymm3, ymm6
	vpminuw ymm7, ymm6
	vpminuw ymm1, ymm4, yword [0x459]
	vpminuw xmm13, xmm9, xmm13
	vpminuw ymm10, ymm15, ymm14
	vpminuw xmm14, xmm8, xmm14
	vpminuw ymm9, ymm9, ymm12
	vpminuw xmm23, xmm31, xmm21
	vpminuw ymm20, ymm24, ymm26
	vpminuw xmm16, xmm31, xmm17
	vpminuw ymm24, ymm28, ymm30
	vpminuw xmm1{k2}, xmm5, xmm6
	vpminuw ymm3{k2}, ymm4, yword [0x6e7]
	vpminuw zmm3{k5}, zmm2, zword [0xd0f]
	vpminuw xmm1{k5}{z}, xmm3, xmm3
	vpminuw ymm4{k6}{z}, ymm5, ymm7
	vpminuw zmm6{k4}{z}, zmm4, zword [0x523]
	vpminuw xmm5, xmm4, oword [eax+1]
	vpminuw xmm6, xmm0, oword [eax+64]
	vpminuw ymm1, ymm6, yword [eax+1]
	vpminuw ymm5, ymm3, yword [eax+64]
	vpminuw xmm5, xmm1, oword [eax+1]
	vpminuw xmm0, xmm5, oword [eax+64]
	vpminuw ymm0, ymm6, yword [eax+1]
	vpminuw ymm0, ymm3, yword [eax+64]
	vpminuw zmm6, zmm7, zword [eax+1]
	vpminuw zmm3, zmm3, zword [eax+64]
	vpminuw xmm4, xmm7, [0x27a]
	vpminuw ymm5, ymm5, [0x67e]
	vpminuw xmm3, xmm0, [0x824]
	vpminuw ymm1, ymm1, [0x1dc]
	vpminuw zmm6, zmm1, [0x56b]
