default rel
	vfmsub132bf16 xmm0, xmm5, xmm1
	vfmsub132bf16 xmm2, xmm6, xmm7
	vfmsub132bf16 ymm7, ymm5, ymm6
	vfmsub132bf16 ymm4, ymm5, yword [0x2b9]
	vfmsub132bf16 zmm1, zmm7, zword [0x2f9]
	vfmsub132bf16 zmm1, zmm7, zmm1
	vfmsub132bf16 xmm11, xmm12, xmm10
	vfmsub132bf16 ymm11, ymm9, ymm13
	vfmsub132bf16 zmm10, zmm11, zmm9
	vfmsub132bf16 xmm17, xmm25, xmm21
	vfmsub132bf16 ymm19, ymm28, ymm30
	vfmsub132bf16 zmm26, zmm26, zmm31
	vfmsub132bf16 xmm2{k5}, xmm7, oword [0xf90]
	vfmsub132bf16 ymm2{k3}, ymm3, ymm1
	vfmsub132bf16 zmm6{k1}, zmm4, zmm0
	vfmsub132bf16 xmm1{k1}{z}, xmm3, oword [0x602]
	vfmsub132bf16 ymm0{k5}{z}, ymm6, ymm5
	vfmsub132bf16 zmm6{k3}{z}, zmm1, zword [0x5d6]
	vfmsub132bf16 xmm6, xmm3, oword [eax+1]
	vfmsub132bf16 xmm6, xmm7, oword [eax+64]
	vfmsub132bf16 ymm4, ymm1, yword [eax+1]
	vfmsub132bf16 ymm6, ymm2, yword [eax+64]
	vfmsub132bf16 zmm3, zmm2, zword [eax+1]
	vfmsub132bf16 zmm7, zmm7, zword [eax+64]
	vfmsub132bf16 xmm2, xmm6, [0x217]
	vfmsub132bf16 ymm3, ymm0, [0x891]
	vfmsub132bf16 zmm2, zmm4, [0x44c]
