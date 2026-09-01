default rel
	vcmptrueps xmm6, xmm3, xmm4
	vcmptrueps xmm6, xmm4
	vcmptrueps xmm0, xmm5, xmm7
	vcmptrueps ymm0, ymm7, ymm3
	vcmptrueps ymm0, ymm3
	vcmptrueps ymm5, ymm3, ymm6
	vcmptrueps k3, xmm5, xmm7
	vcmptrueps k5, xmm0, xmm1
	vcmptrueps k1, ymm4, yword [0x3de]
	vcmptrueps k2, ymm6, yword [0x335]
	vcmptrueps xmm13, xmm13, xmm15
	vcmptrueps ymm11, ymm12, ymm8
	vcmptrueps k4, xmm11, xmm13
	vcmptrueps k5, ymm11, ymm13
	vcmptrueps k2{k3}, xmm5, xmm2
	vcmptrueps k5{k5}, ymm3, ymm7
	vcmptrueps k4{k1}, zmm1, zmm1
	vcmptrueps k2, zmm4, zmm6, {sae}
	vcmptrueps xmm5, xmm4, oword [eax+1]
	vcmptrueps xmm0, xmm3, oword [eax+64]
	vcmptrueps ymm0, ymm0, yword [eax+1]
	vcmptrueps ymm1, ymm2, yword [eax+64]
	vcmptrueps k2, xmm2, oword [eax+1]
	vcmptrueps k1, xmm5, oword [eax+64]
	vcmptrueps k7, ymm6, yword [eax+1]
	vcmptrueps k6, ymm0, yword [eax+64]
	vcmptrueps k6, zmm1, zword [eax+1]
	vcmptrueps k5, zmm7, zword [eax+64]
	vcmptrueps xmm0, xmm6, [0xf83]
	vcmptrueps ymm6, ymm0, [0xcb0]
	vcmptrueps k5, xmm4, [0x22d]
	vcmptrueps k6, ymm1, [0x4ef]
	vcmptrueps k5, zmm1, [0x844]
