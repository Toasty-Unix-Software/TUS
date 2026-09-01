default rel
	vfmadd213bf16 xmm4, xmm6, oword [0x1c3]
	vfmadd213bf16 xmm1, xmm1, oword [0x2ff]
	vfmadd213bf16 ymm3, ymm5, ymm5
	vfmadd213bf16 ymm6, ymm6, ymm2
	vfmadd213bf16 zmm7, zmm2, zmm4
	vfmadd213bf16 zmm6, zmm3, zword [0xabc]
	vfmadd213bf16 xmm9, xmm14, xmm15
	vfmadd213bf16 ymm11, ymm12, ymm8
	vfmadd213bf16 zmm13, zmm9, zmm10
	vfmadd213bf16 xmm24, xmm23, xmm24
	vfmadd213bf16 ymm28, ymm26, ymm25
	vfmadd213bf16 zmm24, zmm18, zmm20
	vfmadd213bf16 xmm4{k5}, xmm0, oword [0x902]
	vfmadd213bf16 ymm5{k2}, ymm5, yword [0xe6b]
	vfmadd213bf16 zmm0{k7}, zmm7, zmm7
	vfmadd213bf16 xmm5{k2}{z}, xmm0, xmm6
	vfmadd213bf16 ymm1{k2}{z}, ymm7, ymm0
	vfmadd213bf16 zmm2{k4}{z}, zmm6, zmm6
	vfmadd213bf16 xmm7, xmm0, oword [eax+1]
	vfmadd213bf16 xmm0, xmm1, oword [eax+64]
	vfmadd213bf16 ymm4, ymm7, yword [eax+1]
	vfmadd213bf16 ymm5, ymm1, yword [eax+64]
	vfmadd213bf16 zmm7, zmm3, zword [eax+1]
	vfmadd213bf16 zmm1, zmm6, zword [eax+64]
	vfmadd213bf16 xmm5, xmm1, [0x434]
	vfmadd213bf16 ymm2, ymm1, [0xc26]
	vfmadd213bf16 zmm2, zmm0, [0xb3b]
