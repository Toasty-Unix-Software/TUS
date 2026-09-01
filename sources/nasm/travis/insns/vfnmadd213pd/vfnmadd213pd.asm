default rel
	vfnmadd213pd xmm1, xmm7, xmm1
	vfnmadd213pd xmm5, xmm1, oword [0xcda]
	vfnmadd213pd ymm0, ymm0, ymm3
	vfnmadd213pd ymm7, ymm6, ymm3
	vfnmadd213pd xmm0, xmm1, xmm5
	vfnmadd213pd xmm6, xmm0, oword [0xab1]
	vfnmadd213pd ymm0, ymm7, ymm3
	vfnmadd213pd ymm2, ymm6, ymm5
	vfnmadd213pd xmm9, xmm8, xmm9
	vfnmadd213pd ymm15, ymm15, ymm12
	vfnmadd213pd xmm14, xmm15, xmm10
	vfnmadd213pd ymm12, ymm14, ymm12
	vfnmadd213pd xmm23, xmm21, xmm16
	vfnmadd213pd ymm19, ymm30, ymm21
	vfnmadd213pd xmm27, xmm30, xmm22
	vfnmadd213pd ymm21, ymm26, ymm23
	vfnmadd213pd xmm2{k6}, xmm5, xmm4
	vfnmadd213pd ymm7{k5}, ymm7, ymm1
	vfnmadd213pd zmm0{k7}, zmm2, zword [0x73c]
	vfnmadd213pd xmm0{k1}{z}, xmm6, xmm5
	vfnmadd213pd ymm4{k1}{z}, ymm3, ymm6
	vfnmadd213pd zmm6{k3}{z}, zmm7, zmm4
	vfnmadd213pd zmm6, zmm4, zmm3, {ru-sae}
	vfnmadd213pd xmm1, xmm3, oword [eax+1]
	vfnmadd213pd xmm2, xmm3, oword [eax+64]
	vfnmadd213pd ymm5, ymm2, yword [eax+1]
	vfnmadd213pd ymm4, ymm3, yword [eax+64]
	vfnmadd213pd xmm5, xmm4, oword [eax+1]
	vfnmadd213pd xmm5, xmm6, oword [eax+64]
	vfnmadd213pd ymm6, ymm6, yword [eax+1]
	vfnmadd213pd ymm2, ymm6, yword [eax+64]
	vfnmadd213pd zmm6, zmm5, zword [eax+1]
	vfnmadd213pd zmm2, zmm4, zword [eax+64]
	vfnmadd213pd xmm4, xmm5, [0x4f7]
	vfnmadd213pd ymm5, ymm4, [0xbf5]
	vfnmadd213pd xmm2, xmm5, [0x8cb]
	vfnmadd213pd ymm6, ymm6, [0xf88]
	vfnmadd213pd zmm7, zmm0, [0x221]
