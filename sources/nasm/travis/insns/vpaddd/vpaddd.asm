default rel
	vpaddd xmm7, xmm3, xmm0
	vpaddd xmm7, xmm0
	vpaddd xmm4, xmm5, xmm6
	vpaddd ymm6, ymm5, yword [0x893]
	vpaddd ymm6, yword [0x893]
	vpaddd ymm4, ymm2, yword [0x41a]
	vpaddd xmm5, xmm3, xmm1
	vpaddd xmm5, xmm1
	vpaddd xmm1, xmm3, xmm4
	vpaddd ymm2, ymm7, ymm7
	vpaddd ymm2, ymm7
	vpaddd ymm2, ymm7, ymm2
	vpaddd xmm15, xmm14, xmm8
	vpaddd ymm10, ymm9, ymm12
	vpaddd xmm8, xmm13, xmm15
	vpaddd ymm9, ymm9, ymm14
	vpaddd xmm31, xmm23, xmm16
	vpaddd ymm31, ymm30, ymm30
	vpaddd xmm19, xmm31, xmm18
	vpaddd ymm24, ymm30, ymm23
	vpaddd xmm0{k3}, xmm0, xmm3
	vpaddd ymm6{k3}, ymm3, yword [0x4be]
	vpaddd zmm4{k7}, zmm0, zmm7
	vpaddd xmm0{k2}{z}, xmm1, xmm5
	vpaddd ymm5{k7}{z}, ymm3, ymm3
	vpaddd zmm6{k2}{z}, zmm0, zword [0xc4b]
	vpaddd xmm0, xmm7, oword [eax+1]
	vpaddd xmm6, xmm2, oword [eax+64]
	vpaddd ymm5, ymm3, yword [eax+1]
	vpaddd ymm6, ymm6, yword [eax+64]
	vpaddd xmm0, xmm5, oword [eax+1]
	vpaddd xmm0, xmm3, oword [eax+64]
	vpaddd ymm1, ymm7, yword [eax+1]
	vpaddd ymm4, ymm5, yword [eax+64]
	vpaddd zmm5, zmm4, zword [eax+1]
	vpaddd zmm6, zmm2, zword [eax+64]
	vpaddd xmm4, xmm1, [0x36b]
	vpaddd ymm4, ymm2, [0x8be]
	vpaddd xmm3, xmm7, [0x261]
	vpaddd ymm0, ymm6, [0xf17]
	vpaddd zmm7, zmm1, [0xcb3]
