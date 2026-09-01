default rel
	vfnmsub231bf16 xmm7, xmm0, xmm6
	vfnmsub231bf16 xmm1, xmm1, xmm2
	vfnmsub231bf16 ymm1, ymm4, yword [0xb6e]
	vfnmsub231bf16 ymm5, ymm6, ymm0
	vfnmsub231bf16 zmm7, zmm6, zmm5
	vfnmsub231bf16 zmm0, zmm6, zword [0xee3]
	vfnmsub231bf16 xmm11, xmm14, xmm12
	vfnmsub231bf16 ymm14, ymm13, ymm10
	vfnmsub231bf16 zmm13, zmm8, zmm14
	vfnmsub231bf16 xmm25, xmm19, xmm26
	vfnmsub231bf16 ymm21, ymm26, ymm25
	vfnmsub231bf16 zmm30, zmm23, zmm30
	vfnmsub231bf16 xmm0{k2}, xmm1, oword [0xf24]
	vfnmsub231bf16 ymm3{k2}, ymm3, ymm1
	vfnmsub231bf16 zmm5{k3}, zmm1, zmm0
	vfnmsub231bf16 xmm6{k1}{z}, xmm0, xmm7
	vfnmsub231bf16 ymm3{k4}{z}, ymm7, yword [0x7a4]
	vfnmsub231bf16 zmm1{k4}{z}, zmm1, zmm6
	vfnmsub231bf16 xmm1, xmm0, oword [eax+1]
	vfnmsub231bf16 xmm5, xmm4, oword [eax+64]
	vfnmsub231bf16 ymm7, ymm2, yword [eax+1]
	vfnmsub231bf16 ymm2, ymm4, yword [eax+64]
	vfnmsub231bf16 zmm0, zmm4, zword [eax+1]
	vfnmsub231bf16 zmm6, zmm6, zword [eax+64]
	vfnmsub231bf16 xmm6, xmm4, [0x604]
	vfnmsub231bf16 ymm2, ymm6, [0x698]
	vfnmsub231bf16 zmm7, zmm5, [0x7e9]
