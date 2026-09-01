default rel
	vfmadd213pd xmm6, xmm0, xmm3
	vfmadd213pd xmm5, xmm1, xmm5
	vfmadd213pd ymm6, ymm6, yword [0xde9]
	vfmadd213pd ymm1, ymm4, ymm7
	vfmadd213pd xmm1, xmm4, oword [0x6f8]
	vfmadd213pd xmm4, xmm4, xmm4
	vfmadd213pd ymm3, ymm5, yword [0xbb2]
	vfmadd213pd ymm7, ymm6, yword [0xa78]
	vfmadd213pd xmm8, xmm12, xmm10
	vfmadd213pd ymm13, ymm10, ymm11
	vfmadd213pd xmm13, xmm12, xmm8
	vfmadd213pd ymm11, ymm15, ymm14
	vfmadd213pd xmm16, xmm19, xmm29
	vfmadd213pd ymm27, ymm18, ymm25
	vfmadd213pd xmm26, xmm20, xmm22
	vfmadd213pd ymm26, ymm28, ymm18
	vfmadd213pd xmm6{k1}, xmm7, xmm7
	vfmadd213pd ymm6{k5}, ymm1, ymm6
	vfmadd213pd zmm0{k4}, zmm2, zword [0x36c]
	vfmadd213pd xmm1{k4}{z}, xmm2, xmm2
	vfmadd213pd ymm5{k7}{z}, ymm6, ymm1
	vfmadd213pd zmm1{k2}{z}, zmm7, zword [0xccb]
	vfmadd213pd zmm6, zmm0, zmm4, {rd-sae}
	vfmadd213pd xmm0, xmm1, oword [eax+1]
	vfmadd213pd xmm2, xmm7, oword [eax+64]
	vfmadd213pd ymm4, ymm1, yword [eax+1]
	vfmadd213pd ymm4, ymm0, yword [eax+64]
	vfmadd213pd xmm5, xmm3, oword [eax+1]
	vfmadd213pd xmm2, xmm7, oword [eax+64]
	vfmadd213pd ymm7, ymm2, yword [eax+1]
	vfmadd213pd ymm3, ymm0, yword [eax+64]
	vfmadd213pd zmm2, zmm2, zword [eax+1]
	vfmadd213pd zmm1, zmm0, zword [eax+64]
	vfmadd213pd xmm6, xmm3, [0x8bb]
	vfmadd213pd ymm7, ymm4, [0xf3e]
	vfmadd213pd xmm1, xmm6, [0x3e8]
	vfmadd213pd ymm1, ymm4, [0x403]
	vfmadd213pd zmm2, zmm1, [0xcd6]
