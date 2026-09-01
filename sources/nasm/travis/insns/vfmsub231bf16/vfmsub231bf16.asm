default rel
	vfmsub231bf16 xmm4, xmm2, xmm1
	vfmsub231bf16 xmm4, xmm4, xmm5
	vfmsub231bf16 ymm2, ymm5, ymm1
	vfmsub231bf16 ymm1, ymm3, ymm1
	vfmsub231bf16 zmm0, zmm3, zmm2
	vfmsub231bf16 zmm4, zmm0, zmm7
	vfmsub231bf16 xmm15, xmm9, xmm11
	vfmsub231bf16 ymm9, ymm9, ymm11
	vfmsub231bf16 zmm8, zmm10, zmm9
	vfmsub231bf16 xmm17, xmm24, xmm23
	vfmsub231bf16 ymm19, ymm22, ymm22
	vfmsub231bf16 zmm26, zmm26, zmm23
	vfmsub231bf16 xmm3{k5}, xmm6, xmm5
	vfmsub231bf16 ymm5{k6}, ymm4, ymm6
	vfmsub231bf16 zmm0{k1}, zmm0, zmm6
	vfmsub231bf16 xmm1{k6}{z}, xmm2, xmm1
	vfmsub231bf16 ymm4{k3}{z}, ymm0, yword [0x754]
	vfmsub231bf16 zmm1{k5}{z}, zmm2, zmm1
	vfmsub231bf16 xmm5, xmm2, oword [eax+1]
	vfmsub231bf16 xmm6, xmm2, oword [eax+64]
	vfmsub231bf16 ymm7, ymm4, yword [eax+1]
	vfmsub231bf16 ymm5, ymm3, yword [eax+64]
	vfmsub231bf16 zmm2, zmm1, zword [eax+1]
	vfmsub231bf16 zmm5, zmm0, zword [eax+64]
	vfmsub231bf16 xmm3, xmm5, [0x37f]
	vfmsub231bf16 ymm7, ymm7, [0x9a4]
	vfmsub231bf16 zmm4, zmm0, [0x5a9]
