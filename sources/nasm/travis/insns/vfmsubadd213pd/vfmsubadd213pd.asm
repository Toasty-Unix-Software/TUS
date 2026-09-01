default rel
	vfmsubadd213pd xmm4, xmm1, xmm7
	vfmsubadd213pd xmm0, xmm6, oword [0x127]
	vfmsubadd213pd ymm4, ymm2, yword [0x6bb]
	vfmsubadd213pd ymm5, ymm0, yword [0xa53]
	vfmsubadd213pd xmm1, xmm1, xmm0
	vfmsubadd213pd xmm1, xmm0, xmm5
	vfmsubadd213pd ymm0, ymm4, ymm3
	vfmsubadd213pd ymm5, ymm4, ymm7
	vfmsubadd213pd xmm8, xmm11, xmm12
	vfmsubadd213pd ymm10, ymm10, ymm14
	vfmsubadd213pd xmm12, xmm10, xmm11
	vfmsubadd213pd ymm15, ymm15, ymm13
	vfmsubadd213pd xmm17, xmm24, xmm21
	vfmsubadd213pd ymm27, ymm26, ymm27
	vfmsubadd213pd xmm27, xmm16, xmm31
	vfmsubadd213pd ymm27, ymm29, ymm24
	vfmsubadd213pd xmm7{k1}, xmm6, xmm0
	vfmsubadd213pd ymm4{k1}, ymm4, yword [0xe3f]
	vfmsubadd213pd zmm7{k7}, zmm2, zword [0x5a2]
	vfmsubadd213pd xmm0{k7}{z}, xmm7, xmm5
	vfmsubadd213pd ymm0{k3}{z}, ymm3, ymm3
	vfmsubadd213pd zmm5{k4}{z}, zmm2, zmm2
	vfmsubadd213pd zmm6, zmm6, zmm7, {rn-sae}
	vfmsubadd213pd xmm3, xmm7, oword [eax+1]
	vfmsubadd213pd xmm3, xmm3, oword [eax+64]
	vfmsubadd213pd ymm1, ymm0, yword [eax+1]
	vfmsubadd213pd ymm4, ymm2, yword [eax+64]
	vfmsubadd213pd xmm1, xmm5, oword [eax+1]
	vfmsubadd213pd xmm4, xmm2, oword [eax+64]
	vfmsubadd213pd ymm1, ymm3, yword [eax+1]
	vfmsubadd213pd ymm4, ymm5, yword [eax+64]
	vfmsubadd213pd zmm3, zmm0, zword [eax+1]
	vfmsubadd213pd zmm2, zmm0, zword [eax+64]
	vfmsubadd213pd xmm7, xmm1, [0x98d]
	vfmsubadd213pd ymm1, ymm5, [0xe5f]
	vfmsubadd213pd xmm3, xmm1, [0x556]
	vfmsubadd213pd ymm2, ymm2, [0x3f9]
	vfmsubadd213pd zmm7, zmm7, [0x1aa]
