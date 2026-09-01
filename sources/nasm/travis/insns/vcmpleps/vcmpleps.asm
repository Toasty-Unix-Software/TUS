default rel
	vcmpleps xmm6, xmm1, xmm0
	vcmpleps xmm6, xmm0
	vcmpleps xmm7, xmm6, oword [0xd6a]
	vcmpleps ymm2, ymm7, ymm3
	vcmpleps ymm2, ymm3
	vcmpleps ymm3, ymm5, yword [0x93f]
	vcmpleps k1, xmm6, xmm3
	vcmpleps k3, xmm4, oword [0x3bc]
	vcmpleps k3, ymm1, ymm1
	vcmpleps k1, ymm5, yword [0xaea]
	vcmpleps xmm13, xmm12, xmm15
	vcmpleps ymm13, ymm10, ymm12
	vcmpleps k4, xmm13, xmm8
	vcmpleps k6, ymm14, ymm12
	vcmpleps k2{k6}, xmm0, xmm7
	vcmpleps k2{k4}, ymm5, yword [0xb83]
	vcmpleps k3{k2}, zmm1, zmm2
	vcmpleps k5, zmm0, zmm6, {sae}
	vcmpleps xmm3, xmm4, oword [eax+1]
	vcmpleps xmm1, xmm3, oword [eax+64]
	vcmpleps ymm2, ymm4, yword [eax+1]
	vcmpleps ymm4, ymm3, yword [eax+64]
	vcmpleps k7, xmm3, oword [eax+1]
	vcmpleps k4, xmm5, oword [eax+64]
	vcmpleps k5, ymm1, yword [eax+1]
	vcmpleps k5, ymm6, yword [eax+64]
	vcmpleps k5, zmm6, zword [eax+1]
	vcmpleps k7, zmm7, zword [eax+64]
	vcmpleps xmm0, xmm4, [0x2df]
	vcmpleps ymm6, ymm0, [0xc4c]
	vcmpleps k5, xmm5, [0xfe7]
	vcmpleps k5, ymm1, [0x57f]
	vcmpleps k5, zmm5, [0x91e]
