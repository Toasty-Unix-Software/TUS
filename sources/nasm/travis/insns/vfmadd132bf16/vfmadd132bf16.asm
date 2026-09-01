default rel
	vfmadd132bf16 xmm1, xmm5, xmm5
	vfmadd132bf16 xmm2, xmm6, xmm3
	vfmadd132bf16 ymm6, ymm7, yword [0xb04]
	vfmadd132bf16 ymm7, ymm4, ymm4
	vfmadd132bf16 zmm0, zmm4, zword [0x5fc]
	vfmadd132bf16 zmm2, zmm7, zmm7
	vfmadd132bf16 xmm12, xmm12, xmm12
	vfmadd132bf16 ymm13, ymm12, ymm8
	vfmadd132bf16 zmm9, zmm15, zmm13
	vfmadd132bf16 xmm27, xmm22, xmm18
	vfmadd132bf16 ymm17, ymm27, ymm19
	vfmadd132bf16 zmm26, zmm23, zmm20
	vfmadd132bf16 xmm6{k3}, xmm1, xmm7
	vfmadd132bf16 ymm5{k7}, ymm5, ymm1
	vfmadd132bf16 zmm0{k4}, zmm3, zmm6
	vfmadd132bf16 xmm6{k5}{z}, xmm1, xmm3
	vfmadd132bf16 ymm6{k1}{z}, ymm0, yword [0xa32]
	vfmadd132bf16 zmm0{k7}{z}, zmm7, zmm3
	vfmadd132bf16 xmm2, xmm2, oword [eax+1]
	vfmadd132bf16 xmm7, xmm5, oword [eax+64]
	vfmadd132bf16 ymm4, ymm3, yword [eax+1]
	vfmadd132bf16 ymm7, ymm1, yword [eax+64]
	vfmadd132bf16 zmm0, zmm7, zword [eax+1]
	vfmadd132bf16 zmm0, zmm0, zword [eax+64]
	vfmadd132bf16 xmm7, xmm3, [0xa9d]
	vfmadd132bf16 ymm2, ymm4, [0x803]
	vfmadd132bf16 zmm5, zmm5, [0x1f7]
