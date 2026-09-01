default rel
	vfmadd231bf16 xmm6, xmm4, xmm2
	vfmadd231bf16 xmm0, xmm6, xmm6
	vfmadd231bf16 ymm5, ymm4, ymm5
	vfmadd231bf16 ymm6, ymm0, ymm5
	vfmadd231bf16 zmm2, zmm7, zmm6
	vfmadd231bf16 zmm4, zmm7, zmm6
	vfmadd231bf16 xmm10, xmm15, xmm11
	vfmadd231bf16 ymm10, ymm8, ymm8
	vfmadd231bf16 zmm11, zmm10, zmm11
	vfmadd231bf16 xmm30, xmm22, xmm31
	vfmadd231bf16 ymm31, ymm25, ymm27
	vfmadd231bf16 zmm16, zmm28, zmm22
	vfmadd231bf16 xmm0{k7}, xmm6, oword [0x3cf]
	vfmadd231bf16 ymm1{k3}, ymm6, ymm3
	vfmadd231bf16 zmm2{k7}, zmm0, zmm4
	vfmadd231bf16 xmm2{k6}{z}, xmm7, oword [0xf02]
	vfmadd231bf16 ymm3{k6}{z}, ymm5, ymm3
	vfmadd231bf16 zmm2{k5}{z}, zmm6, zmm0
	vfmadd231bf16 xmm6, xmm2, oword [eax+1]
	vfmadd231bf16 xmm2, xmm3, oword [eax+64]
	vfmadd231bf16 ymm5, ymm4, yword [eax+1]
	vfmadd231bf16 ymm2, ymm5, yword [eax+64]
	vfmadd231bf16 zmm2, zmm2, zword [eax+1]
	vfmadd231bf16 zmm5, zmm4, zword [eax+64]
	vfmadd231bf16 xmm7, xmm6, [0xb65]
	vfmadd231bf16 ymm5, ymm7, [0x3f9]
	vfmadd231bf16 zmm0, zmm0, [0xbc1]
