default rel
	vfnmadd231ph xmm6, xmm3, xmm2
	vfnmadd231ph xmm6, xmm2
	vfnmadd231ph xmm4, xmm0, xmm1
	vfnmadd231ph ymm0, ymm7, ymm3
	vfnmadd231ph ymm0, ymm3
	vfnmadd231ph ymm6, ymm5, ymm6
	vfnmadd231ph zmm2, zmm3, zword [0xace]
	vfnmadd231ph zmm2, zword [0xace]
	vfnmadd231ph zmm0, zmm5, zword [0xf2d]
	vfnmadd231ph xmm9, xmm14, xmm8
	vfnmadd231ph ymm14, ymm10, ymm12
	vfnmadd231ph zmm13, zmm9, zmm8
	vfnmadd231ph xmm16, xmm26, xmm19
	vfnmadd231ph ymm28, ymm22, ymm20
	vfnmadd231ph zmm23, zmm25, zmm17
	vfnmadd231ph xmm5{k7}, xmm0, xmm5
	vfnmadd231ph ymm1{k4}, ymm0, ymm6
	vfnmadd231ph zmm4{k2}, zmm3, zmm1
	vfnmadd231ph xmm1{k7}{z}, xmm4, xmm2
	vfnmadd231ph ymm2{k1}{z}, ymm2, ymm4
	vfnmadd231ph zmm3{k1}{z}, zmm2, zword [0x7b0]
	vfnmadd231ph zmm3, zmm3, zmm5, {rd-sae}
	vfnmadd231ph xmm5, xmm4, oword [eax+1]
	vfnmadd231ph xmm1, xmm4, oword [eax+64]
	vfnmadd231ph ymm6, ymm7, yword [eax+1]
	vfnmadd231ph ymm5, ymm4, yword [eax+64]
	vfnmadd231ph zmm2, zmm1, zword [eax+1]
	vfnmadd231ph zmm2, zmm5, zword [eax+64]
	vfnmadd231ph xmm2, xmm3, [0x8b2]
	vfnmadd231ph ymm3, ymm7, [0xddf]
	vfnmadd231ph zmm2, zmm6, [0x991]
