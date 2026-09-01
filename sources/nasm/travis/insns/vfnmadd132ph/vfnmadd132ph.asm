default rel
	vfnmadd132ph xmm1, xmm7, oword [0xf73]
	vfnmadd132ph xmm1, oword [0xf73]
	vfnmadd132ph xmm2, xmm7, xmm0
	vfnmadd132ph ymm6, ymm1, yword [0xd9a]
	vfnmadd132ph ymm6, yword [0xd9a]
	vfnmadd132ph ymm4, ymm2, ymm7
	vfnmadd132ph zmm4, zmm5, zmm6
	vfnmadd132ph zmm4, zmm6
	vfnmadd132ph zmm7, zmm0, zword [0x6c6]
	vfnmadd132ph xmm10, xmm12, xmm9
	vfnmadd132ph ymm8, ymm12, ymm10
	vfnmadd132ph zmm12, zmm14, zmm12
	vfnmadd132ph xmm20, xmm22, xmm20
	vfnmadd132ph ymm19, ymm18, ymm19
	vfnmadd132ph zmm19, zmm23, zmm24
	vfnmadd132ph xmm4{k2}, xmm1, xmm5
	vfnmadd132ph ymm7{k4}, ymm5, ymm2
	vfnmadd132ph zmm0{k5}, zmm6, zmm0
	vfnmadd132ph xmm7{k6}{z}, xmm1, xmm6
	vfnmadd132ph ymm0{k7}{z}, ymm2, ymm4
	vfnmadd132ph zmm1{k1}{z}, zmm3, zmm1
	vfnmadd132ph zmm7, zmm3, zmm2, {rd-sae}
	vfnmadd132ph xmm1, xmm1, oword [eax+1]
	vfnmadd132ph xmm7, xmm3, oword [eax+64]
	vfnmadd132ph ymm3, ymm3, yword [eax+1]
	vfnmadd132ph ymm2, ymm4, yword [eax+64]
	vfnmadd132ph zmm2, zmm0, zword [eax+1]
	vfnmadd132ph zmm2, zmm5, zword [eax+64]
	vfnmadd132ph xmm1, xmm4, [0xd4f]
	vfnmadd132ph ymm7, ymm5, [0x7b5]
	vfnmadd132ph zmm2, zmm2, [0x1b7]
