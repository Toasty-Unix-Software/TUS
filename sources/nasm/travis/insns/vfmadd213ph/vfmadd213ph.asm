default rel
	vfmadd213ph xmm2, xmm0, oword [0xf56]
	vfmadd213ph xmm2, oword [0xf56]
	vfmadd213ph xmm6, xmm3, xmm0
	vfmadd213ph ymm6, ymm3, ymm2
	vfmadd213ph ymm6, ymm2
	vfmadd213ph ymm0, ymm0, ymm1
	vfmadd213ph zmm7, zmm1, zmm4
	vfmadd213ph zmm7, zmm4
	vfmadd213ph zmm0, zmm6, zmm7
	vfmadd213ph xmm15, xmm12, xmm12
	vfmadd213ph ymm15, ymm10, ymm10
	vfmadd213ph zmm15, zmm14, zmm9
	vfmadd213ph xmm19, xmm31, xmm30
	vfmadd213ph ymm29, ymm30, ymm24
	vfmadd213ph zmm17, zmm19, zmm28
	vfmadd213ph xmm0{k2}, xmm1, xmm2
	vfmadd213ph ymm4{k3}, ymm0, ymm0
	vfmadd213ph zmm0{k5}, zmm4, zword [0x62c]
	vfmadd213ph xmm6{k5}{z}, xmm7, oword [0x264]
	vfmadd213ph ymm0{k2}{z}, ymm5, ymm2
	vfmadd213ph zmm4{k4}{z}, zmm4, zmm3
	vfmadd213ph zmm7, zmm2, zmm0, {rd-sae}
	vfmadd213ph xmm1, xmm3, oword [eax+1]
	vfmadd213ph xmm1, xmm7, oword [eax+64]
	vfmadd213ph ymm1, ymm1, yword [eax+1]
	vfmadd213ph ymm5, ymm4, yword [eax+64]
	vfmadd213ph zmm6, zmm0, zword [eax+1]
	vfmadd213ph zmm2, zmm7, zword [eax+64]
	vfmadd213ph xmm2, xmm6, [0x6e1]
	vfmadd213ph ymm0, ymm0, [0x5a6]
	vfmadd213ph zmm1, zmm6, [0x40c]
