default rel
	vfmsub213pd xmm6, xmm0, xmm2
	vfmsub213pd xmm5, xmm7, xmm1
	vfmsub213pd ymm2, ymm4, ymm3
	vfmsub213pd ymm2, ymm2, ymm1
	vfmsub213pd xmm1, xmm0, oword [0x260]
	vfmsub213pd xmm2, xmm0, xmm2
	vfmsub213pd ymm3, ymm1, yword [0x7d0]
	vfmsub213pd ymm2, ymm5, ymm3
	vfmsub213pd xmm10, xmm9, xmm15
	vfmsub213pd ymm9, ymm13, ymm14
	vfmsub213pd xmm14, xmm11, xmm9
	vfmsub213pd ymm13, ymm15, ymm12
	vfmsub213pd xmm16, xmm31, xmm16
	vfmsub213pd ymm17, ymm24, ymm23
	vfmsub213pd xmm30, xmm27, xmm19
	vfmsub213pd ymm30, ymm29, ymm16
	vfmsub213pd xmm3{k4}, xmm2, xmm2
	vfmsub213pd ymm1{k4}, ymm3, yword [0x6ba]
	vfmsub213pd zmm0{k7}, zmm0, zword [0x631]
	vfmsub213pd xmm7{k5}{z}, xmm2, xmm3
	vfmsub213pd ymm6{k3}{z}, ymm1, ymm3
	vfmsub213pd zmm1{k2}{z}, zmm4, zmm1
	vfmsub213pd zmm0, zmm0, zmm6, {rd-sae}
	vfmsub213pd xmm7, xmm2, oword [eax+1]
	vfmsub213pd xmm7, xmm1, oword [eax+64]
	vfmsub213pd ymm6, ymm4, yword [eax+1]
	vfmsub213pd ymm2, ymm4, yword [eax+64]
	vfmsub213pd xmm4, xmm3, oword [eax+1]
	vfmsub213pd xmm3, xmm3, oword [eax+64]
	vfmsub213pd ymm2, ymm7, yword [eax+1]
	vfmsub213pd ymm2, ymm3, yword [eax+64]
	vfmsub213pd zmm4, zmm7, zword [eax+1]
	vfmsub213pd zmm1, zmm3, zword [eax+64]
	vfmsub213pd xmm4, xmm7, [0xd69]
	vfmsub213pd ymm7, ymm5, [0xec4]
	vfmsub213pd xmm0, xmm1, [0xc5f]
	vfmsub213pd ymm1, ymm3, [0x75e]
	vfmsub213pd zmm1, zmm5, [0x5c7]
