default rel
	vfnmsub132bf16 xmm5, xmm0, xmm4
	vfnmsub132bf16 xmm2, xmm4, xmm5
	vfnmsub132bf16 ymm2, ymm3, ymm4
	vfnmsub132bf16 ymm4, ymm0, ymm5
	vfnmsub132bf16 zmm7, zmm6, zword [0x819]
	vfnmsub132bf16 zmm6, zmm6, zword [0xaee]
	vfnmsub132bf16 xmm11, xmm8, xmm13
	vfnmsub132bf16 ymm14, ymm13, ymm14
	vfnmsub132bf16 zmm8, zmm9, zmm14
	vfnmsub132bf16 xmm16, xmm24, xmm19
	vfnmsub132bf16 ymm19, ymm26, ymm28
	vfnmsub132bf16 zmm31, zmm19, zmm20
	vfnmsub132bf16 xmm2{k4}, xmm1, xmm0
	vfnmsub132bf16 ymm2{k6}, ymm7, ymm4
	vfnmsub132bf16 zmm3{k4}, zmm5, zword [0x34f]
	vfnmsub132bf16 xmm6{k3}{z}, xmm5, xmm3
	vfnmsub132bf16 ymm5{k1}{z}, ymm0, ymm1
	vfnmsub132bf16 zmm7{k7}{z}, zmm3, zmm7
	vfnmsub132bf16 xmm4, xmm1, oword [eax+1]
	vfnmsub132bf16 xmm3, xmm2, oword [eax+64]
	vfnmsub132bf16 ymm2, ymm4, yword [eax+1]
	vfnmsub132bf16 ymm4, ymm6, yword [eax+64]
	vfnmsub132bf16 zmm6, zmm6, zword [eax+1]
	vfnmsub132bf16 zmm2, zmm5, zword [eax+64]
	vfnmsub132bf16 xmm3, xmm1, [0x1d2]
	vfnmsub132bf16 ymm4, ymm7, [0x225]
	vfnmsub132bf16 zmm2, zmm4, [0x9a6]
