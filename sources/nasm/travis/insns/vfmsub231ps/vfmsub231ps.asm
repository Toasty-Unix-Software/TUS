default rel
	vfmsub231ps xmm5, xmm7, xmm3
	vfmsub231ps xmm5, xmm4, xmm0
	vfmsub231ps ymm4, ymm2, ymm5
	vfmsub231ps ymm4, ymm7, ymm4
	vfmsub231ps xmm3, xmm6, xmm6
	vfmsub231ps xmm6, xmm7, xmm7
	vfmsub231ps ymm2, ymm4, yword [0xed4]
	vfmsub231ps ymm5, ymm7, ymm5
	vfmsub231ps xmm15, xmm13, xmm11
	vfmsub231ps ymm11, ymm10, ymm15
	vfmsub231ps xmm15, xmm12, xmm14
	vfmsub231ps ymm14, ymm15, ymm12
	vfmsub231ps xmm21, xmm25, xmm25
	vfmsub231ps ymm16, ymm18, ymm28
	vfmsub231ps xmm27, xmm31, xmm26
	vfmsub231ps ymm25, ymm28, ymm21
	vfmsub231ps xmm4{k1}, xmm2, xmm6
	vfmsub231ps ymm5{k4}, ymm3, ymm4
	vfmsub231ps zmm3{k6}, zmm1, zmm3
	vfmsub231ps xmm5{k4}{z}, xmm7, xmm7
	vfmsub231ps ymm4{k2}{z}, ymm0, ymm6
	vfmsub231ps zmm1{k6}{z}, zmm0, zmm2
	vfmsub231ps zmm6, zmm3, zmm2, {rn-sae}
	vfmsub231ps xmm4, xmm7, oword [eax+1]
	vfmsub231ps xmm3, xmm4, oword [eax+64]
	vfmsub231ps ymm2, ymm0, yword [eax+1]
	vfmsub231ps ymm7, ymm0, yword [eax+64]
	vfmsub231ps xmm3, xmm2, oword [eax+1]
	vfmsub231ps xmm4, xmm5, oword [eax+64]
	vfmsub231ps ymm4, ymm4, yword [eax+1]
	vfmsub231ps ymm6, ymm0, yword [eax+64]
	vfmsub231ps zmm3, zmm0, zword [eax+1]
	vfmsub231ps zmm3, zmm1, zword [eax+64]
	vfmsub231ps xmm7, xmm3, [0xa59]
	vfmsub231ps ymm7, ymm3, [0x448]
	vfmsub231ps xmm6, xmm5, [0xb50]
	vfmsub231ps ymm7, ymm1, [0xf5e]
	vfmsub231ps zmm1, zmm2, [0x1d8]
