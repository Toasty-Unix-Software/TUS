default rel
	vfmsubadd231pd xmm2, xmm3, oword [0x2a7]
	vfmsubadd231pd xmm6, xmm7, xmm7
	vfmsubadd231pd ymm0, ymm3, ymm6
	vfmsubadd231pd ymm6, ymm5, ymm0
	vfmsubadd231pd xmm2, xmm5, xmm3
	vfmsubadd231pd xmm1, xmm1, oword [0x396]
	vfmsubadd231pd ymm0, ymm7, ymm3
	vfmsubadd231pd ymm5, ymm0, ymm1
	vfmsubadd231pd xmm15, xmm12, xmm13
	vfmsubadd231pd ymm9, ymm12, ymm14
	vfmsubadd231pd xmm10, xmm9, xmm12
	vfmsubadd231pd ymm12, ymm8, ymm10
	vfmsubadd231pd xmm22, xmm18, xmm31
	vfmsubadd231pd ymm25, ymm28, ymm28
	vfmsubadd231pd xmm30, xmm24, xmm18
	vfmsubadd231pd ymm26, ymm28, ymm26
	vfmsubadd231pd xmm2{k7}, xmm5, xmm7
	vfmsubadd231pd ymm1{k5}, ymm2, ymm1
	vfmsubadd231pd zmm2{k7}, zmm0, zmm0
	vfmsubadd231pd xmm7{k4}{z}, xmm3, xmm6
	vfmsubadd231pd ymm5{k7}{z}, ymm7, ymm0
	vfmsubadd231pd zmm6{k1}{z}, zmm1, zmm1
	vfmsubadd231pd zmm6, zmm0, zmm7, {rn-sae}
	vfmsubadd231pd xmm3, xmm3, oword [eax+1]
	vfmsubadd231pd xmm1, xmm4, oword [eax+64]
	vfmsubadd231pd ymm7, ymm7, yword [eax+1]
	vfmsubadd231pd ymm0, ymm1, yword [eax+64]
	vfmsubadd231pd xmm7, xmm4, oword [eax+1]
	vfmsubadd231pd xmm6, xmm0, oword [eax+64]
	vfmsubadd231pd ymm5, ymm2, yword [eax+1]
	vfmsubadd231pd ymm7, ymm6, yword [eax+64]
	vfmsubadd231pd zmm1, zmm2, zword [eax+1]
	vfmsubadd231pd zmm0, zmm5, zword [eax+64]
	vfmsubadd231pd xmm3, xmm6, [0x58f]
	vfmsubadd231pd ymm4, ymm1, [0xe88]
	vfmsubadd231pd xmm7, xmm7, [0xd21]
	vfmsubadd231pd ymm3, ymm6, [0x48d]
	vfmsubadd231pd zmm4, zmm1, [0x7a3]
