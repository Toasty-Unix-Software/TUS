default rel
	vfmadd231pd xmm5, xmm7, xmm6
	vfmadd231pd xmm2, xmm3, xmm7
	vfmadd231pd ymm6, ymm7, ymm2
	vfmadd231pd ymm2, ymm4, ymm2
	vfmadd231pd xmm0, xmm3, xmm7
	vfmadd231pd xmm7, xmm2, oword [0x763]
	vfmadd231pd ymm5, ymm2, yword [0xa0c]
	vfmadd231pd ymm1, ymm3, yword [0x80f]
	vfmadd231pd xmm10, xmm15, xmm14
	vfmadd231pd ymm15, ymm8, ymm14
	vfmadd231pd xmm13, xmm9, xmm15
	vfmadd231pd ymm9, ymm8, ymm9
	vfmadd231pd xmm16, xmm31, xmm25
	vfmadd231pd ymm16, ymm29, ymm24
	vfmadd231pd xmm21, xmm24, xmm22
	vfmadd231pd ymm21, ymm23, ymm30
	vfmadd231pd xmm5{k1}, xmm7, xmm4
	vfmadd231pd ymm4{k7}, ymm3, ymm7
	vfmadd231pd zmm5{k7}, zmm5, zmm5
	vfmadd231pd xmm4{k3}{z}, xmm1, xmm6
	vfmadd231pd ymm7{k5}{z}, ymm7, ymm3
	vfmadd231pd zmm0{k5}{z}, zmm1, zmm3
	vfmadd231pd zmm3, zmm1, zmm0, {rz-sae}
	vfmadd231pd xmm3, xmm3, oword [eax+1]
	vfmadd231pd xmm3, xmm6, oword [eax+64]
	vfmadd231pd ymm5, ymm3, yword [eax+1]
	vfmadd231pd ymm0, ymm4, yword [eax+64]
	vfmadd231pd xmm4, xmm6, oword [eax+1]
	vfmadd231pd xmm5, xmm6, oword [eax+64]
	vfmadd231pd ymm7, ymm6, yword [eax+1]
	vfmadd231pd ymm6, ymm3, yword [eax+64]
	vfmadd231pd zmm1, zmm0, zword [eax+1]
	vfmadd231pd zmm7, zmm0, zword [eax+64]
	vfmadd231pd xmm1, xmm1, [0xc85]
	vfmadd231pd ymm5, ymm0, [0x282]
	vfmadd231pd xmm5, xmm5, [0x51d]
	vfmadd231pd ymm5, ymm5, [0xea5]
	vfmadd231pd zmm7, zmm1, [0x76e]
