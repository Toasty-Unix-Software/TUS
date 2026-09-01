default rel
	vfnmadd231pd xmm4, xmm5, xmm0
	vfnmadd231pd xmm0, xmm3, xmm1
	vfnmadd231pd ymm5, ymm4, ymm4
	vfnmadd231pd ymm7, ymm2, yword [0x563]
	vfnmadd231pd xmm5, xmm2, xmm5
	vfnmadd231pd xmm3, xmm0, oword [0xd65]
	vfnmadd231pd ymm0, ymm4, ymm1
	vfnmadd231pd ymm0, ymm7, ymm4
	vfnmadd231pd xmm10, xmm13, xmm14
	vfnmadd231pd ymm13, ymm8, ymm12
	vfnmadd231pd xmm13, xmm12, xmm15
	vfnmadd231pd ymm12, ymm9, ymm11
	vfnmadd231pd xmm30, xmm16, xmm19
	vfnmadd231pd ymm21, ymm18, ymm28
	vfnmadd231pd xmm30, xmm28, xmm21
	vfnmadd231pd ymm20, ymm18, ymm26
	vfnmadd231pd xmm5{k3}, xmm3, xmm1
	vfnmadd231pd ymm5{k6}, ymm0, yword [0x2fd]
	vfnmadd231pd zmm4{k4}, zmm2, zmm7
	vfnmadd231pd xmm6{k2}{z}, xmm1, xmm7
	vfnmadd231pd ymm6{k6}{z}, ymm1, ymm3
	vfnmadd231pd zmm0{k1}{z}, zmm6, zmm0
	vfnmadd231pd zmm1, zmm7, zmm7, {rz-sae}
	vfnmadd231pd xmm3, xmm3, oword [eax+1]
	vfnmadd231pd xmm0, xmm5, oword [eax+64]
	vfnmadd231pd ymm6, ymm6, yword [eax+1]
	vfnmadd231pd ymm0, ymm7, yword [eax+64]
	vfnmadd231pd xmm2, xmm5, oword [eax+1]
	vfnmadd231pd xmm0, xmm1, oword [eax+64]
	vfnmadd231pd ymm2, ymm6, yword [eax+1]
	vfnmadd231pd ymm4, ymm7, yword [eax+64]
	vfnmadd231pd zmm0, zmm2, zword [eax+1]
	vfnmadd231pd zmm3, zmm3, zword [eax+64]
	vfnmadd231pd xmm1, xmm7, [0x354]
	vfnmadd231pd ymm2, ymm3, [0x2ae]
	vfnmadd231pd xmm2, xmm4, [0x36a]
	vfnmadd231pd ymm3, ymm7, [0x56f]
	vfnmadd231pd zmm1, zmm4, [0xe70]
