default rel
	vpaddb xmm1, xmm4, xmm0
	vpaddb xmm1, xmm0
	vpaddb xmm1, xmm5, xmm3
	vpaddb ymm2, ymm2, ymm7
	vpaddb ymm2, ymm7
	vpaddb ymm4, ymm0, ymm4
	vpaddb xmm0, xmm6, xmm2
	vpaddb xmm0, xmm2
	vpaddb xmm0, xmm2, oword [0x11f]
	vpaddb ymm4, ymm5, ymm0
	vpaddb ymm4, ymm0
	vpaddb ymm5, ymm0, yword [0xd79]
	vpaddb xmm11, xmm12, xmm8
	vpaddb ymm13, ymm9, ymm8
	vpaddb xmm8, xmm11, xmm14
	vpaddb ymm15, ymm9, ymm14
	vpaddb xmm24, xmm28, xmm31
	vpaddb ymm26, ymm23, ymm26
	vpaddb xmm19, xmm20, xmm18
	vpaddb ymm27, ymm28, ymm22
	vpaddb xmm1{k6}, xmm5, xmm1
	vpaddb ymm1{k4}, ymm6, ymm1
	vpaddb zmm5{k6}, zmm6, zword [0x54c]
	vpaddb xmm2{k5}{z}, xmm6, oword [0x370]
	vpaddb ymm6{k3}{z}, ymm3, yword [0xb4c]
	vpaddb zmm1{k4}{z}, zmm4, zword [0x5cc]
	vpaddb xmm3, xmm3, oword [eax+1]
	vpaddb xmm4, xmm7, oword [eax+64]
	vpaddb ymm7, ymm7, yword [eax+1]
	vpaddb ymm7, ymm5, yword [eax+64]
	vpaddb xmm4, xmm7, oword [eax+1]
	vpaddb xmm4, xmm4, oword [eax+64]
	vpaddb ymm5, ymm3, yword [eax+1]
	vpaddb ymm7, ymm4, yword [eax+64]
	vpaddb zmm1, zmm6, zword [eax+1]
	vpaddb zmm7, zmm2, zword [eax+64]
	vpaddb xmm6, xmm0, [0x6e3]
	vpaddb ymm7, ymm1, [0xd0a]
	vpaddb xmm6, xmm2, [0x1ba]
	vpaddb ymm2, ymm2, [0xfe6]
	vpaddb zmm5, zmm7, [0x7ce]
