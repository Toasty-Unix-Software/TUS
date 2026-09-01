default rel
	vfnmadd213bf16 xmm3, xmm2, xmm0
	vfnmadd213bf16 xmm0, xmm3, oword [0xb74]
	vfnmadd213bf16 ymm4, ymm6, ymm5
	vfnmadd213bf16 ymm1, ymm3, yword [0xf59]
	vfnmadd213bf16 zmm5, zmm3, zmm5
	vfnmadd213bf16 zmm7, zmm1, zword [0xf15]
	vfnmadd213bf16 xmm14, xmm10, xmm10
	vfnmadd213bf16 ymm12, ymm11, ymm13
	vfnmadd213bf16 zmm15, zmm12, zmm8
	vfnmadd213bf16 xmm18, xmm17, xmm29
	vfnmadd213bf16 ymm26, ymm30, ymm30
	vfnmadd213bf16 zmm18, zmm23, zmm31
	vfnmadd213bf16 xmm6{k1}, xmm0, oword [0xfe6]
	vfnmadd213bf16 ymm0{k2}, ymm5, yword [0xd69]
	vfnmadd213bf16 zmm5{k4}, zmm1, zmm2
	vfnmadd213bf16 xmm2{k4}{z}, xmm4, xmm6
	vfnmadd213bf16 ymm0{k2}{z}, ymm7, ymm4
	vfnmadd213bf16 zmm7{k4}{z}, zmm1, zmm2
	vfnmadd213bf16 xmm5, xmm1, oword [eax+1]
	vfnmadd213bf16 xmm0, xmm1, oword [eax+64]
	vfnmadd213bf16 ymm6, ymm2, yword [eax+1]
	vfnmadd213bf16 ymm0, ymm5, yword [eax+64]
	vfnmadd213bf16 zmm0, zmm4, zword [eax+1]
	vfnmadd213bf16 zmm5, zmm1, zword [eax+64]
	vfnmadd213bf16 xmm0, xmm2, [0x51f]
	vfnmadd213bf16 ymm2, ymm3, [0xf29]
	vfnmadd213bf16 zmm5, zmm6, [0x737]
