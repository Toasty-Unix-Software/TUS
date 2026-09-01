default rel
	vfnmsub231pd xmm2, xmm5, xmm6
	vfnmsub231pd xmm3, xmm6, xmm5
	vfnmsub231pd ymm7, ymm5, ymm3
	vfnmsub231pd ymm3, ymm6, ymm5
	vfnmsub231pd xmm0, xmm0, xmm0
	vfnmsub231pd xmm7, xmm4, xmm1
	vfnmsub231pd ymm0, ymm5, yword [0x889]
	vfnmsub231pd ymm4, ymm7, ymm0
	vfnmsub231pd xmm12, xmm14, xmm11
	vfnmsub231pd ymm9, ymm10, ymm10
	vfnmsub231pd xmm10, xmm14, xmm9
	vfnmsub231pd ymm14, ymm10, ymm11
	vfnmsub231pd xmm30, xmm25, xmm20
	vfnmsub231pd ymm18, ymm16, ymm30
	vfnmsub231pd xmm18, xmm21, xmm16
	vfnmsub231pd ymm28, ymm23, ymm23
	vfnmsub231pd xmm4{k7}, xmm0, xmm5
	vfnmsub231pd ymm0{k2}, ymm2, ymm6
	vfnmsub231pd zmm5{k3}, zmm7, zword [0x308]
	vfnmsub231pd xmm1{k7}{z}, xmm3, xmm4
	vfnmsub231pd ymm0{k5}{z}, ymm4, ymm5
	vfnmsub231pd zmm2{k7}{z}, zmm5, zmm3
	vfnmsub231pd zmm7, zmm5, zmm3, {rd-sae}
	vfnmsub231pd xmm7, xmm6, oword [eax+1]
	vfnmsub231pd xmm4, xmm7, oword [eax+64]
	vfnmsub231pd ymm4, ymm2, yword [eax+1]
	vfnmsub231pd ymm7, ymm2, yword [eax+64]
	vfnmsub231pd xmm5, xmm2, oword [eax+1]
	vfnmsub231pd xmm3, xmm2, oword [eax+64]
	vfnmsub231pd ymm2, ymm0, yword [eax+1]
	vfnmsub231pd ymm0, ymm1, yword [eax+64]
	vfnmsub231pd zmm5, zmm5, zword [eax+1]
	vfnmsub231pd zmm1, zmm2, zword [eax+64]
	vfnmsub231pd xmm0, xmm7, [0x66f]
	vfnmsub231pd ymm2, ymm2, [0x516]
	vfnmsub231pd xmm2, xmm4, [0x3fe]
	vfnmsub231pd ymm5, ymm5, [0xc9c]
	vfnmsub231pd zmm2, zmm5, [0xd37]
