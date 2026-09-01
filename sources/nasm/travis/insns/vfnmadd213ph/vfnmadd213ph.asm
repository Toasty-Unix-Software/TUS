default rel
	vfnmadd213ph xmm6, xmm0, xmm0
	vfnmadd213ph xmm6, xmm0
	vfnmadd213ph xmm7, xmm1, xmm3
	vfnmadd213ph ymm1, ymm3, ymm3
	vfnmadd213ph ymm1, ymm3
	vfnmadd213ph ymm7, ymm7, ymm6
	vfnmadd213ph zmm0, zmm5, zmm4
	vfnmadd213ph zmm0, zmm4
	vfnmadd213ph zmm7, zmm5, zmm1
	vfnmadd213ph xmm15, xmm14, xmm9
	vfnmadd213ph ymm13, ymm8, ymm9
	vfnmadd213ph zmm8, zmm10, zmm14
	vfnmadd213ph xmm18, xmm19, xmm30
	vfnmadd213ph ymm27, ymm20, ymm30
	vfnmadd213ph zmm27, zmm29, zmm17
	vfnmadd213ph xmm5{k6}, xmm4, oword [0xeb0]
	vfnmadd213ph ymm2{k6}, ymm4, ymm0
	vfnmadd213ph zmm3{k7}, zmm1, zmm6
	vfnmadd213ph xmm2{k4}{z}, xmm0, oword [0xed1]
	vfnmadd213ph ymm2{k5}{z}, ymm1, ymm6
	vfnmadd213ph zmm0{k4}{z}, zmm2, zmm6
	vfnmadd213ph zmm2, zmm1, zmm6, {ru-sae}
	vfnmadd213ph xmm0, xmm7, oword [eax+1]
	vfnmadd213ph xmm2, xmm4, oword [eax+64]
	vfnmadd213ph ymm7, ymm5, yword [eax+1]
	vfnmadd213ph ymm7, ymm7, yword [eax+64]
	vfnmadd213ph zmm1, zmm7, zword [eax+1]
	vfnmadd213ph zmm0, zmm5, zword [eax+64]
	vfnmadd213ph xmm2, xmm7, [0xba9]
	vfnmadd213ph ymm7, ymm7, [0x1bb]
	vfnmadd213ph zmm0, zmm0, [0x4d5]
