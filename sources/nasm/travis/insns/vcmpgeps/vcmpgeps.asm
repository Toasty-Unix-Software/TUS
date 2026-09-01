default rel
	vcmpgeps xmm3, xmm1, oword [0xf31]
	vcmpgeps xmm3, oword [0xf31]
	vcmpgeps xmm1, xmm5, oword [0x9ba]
	vcmpgeps ymm2, ymm3, ymm7
	vcmpgeps ymm2, ymm7
	vcmpgeps ymm1, ymm1, yword [0x14e]
	vcmpgeps k7, xmm3, xmm4
	vcmpgeps k4, xmm6, xmm1
	vcmpgeps k3, ymm0, ymm0
	vcmpgeps k1, ymm3, ymm6
	vcmpgeps xmm15, xmm15, xmm15
	vcmpgeps ymm13, ymm9, ymm15
	vcmpgeps k1, xmm8, xmm12
	vcmpgeps k5, ymm13, ymm14
	vcmpgeps k4{k3}, xmm1, xmm6
	vcmpgeps k3{k7}, ymm4, ymm2
	vcmpgeps k4{k1}, zmm0, zmm3
	vcmpgeps k7, zmm0, zmm2, {sae}
	vcmpgeps xmm7, xmm1, oword [eax+1]
	vcmpgeps xmm0, xmm6, oword [eax+64]
	vcmpgeps ymm0, ymm5, yword [eax+1]
	vcmpgeps ymm6, ymm4, yword [eax+64]
	vcmpgeps k2, xmm7, oword [eax+1]
	vcmpgeps k7, xmm2, oword [eax+64]
	vcmpgeps k2, ymm6, yword [eax+1]
	vcmpgeps k1, ymm0, yword [eax+64]
	vcmpgeps k3, zmm1, zword [eax+1]
	vcmpgeps k6, zmm7, zword [eax+64]
	vcmpgeps xmm7, xmm5, [0x800]
	vcmpgeps ymm4, ymm5, [0xb9b]
	vcmpgeps k6, xmm6, [0x226]
	vcmpgeps k2, ymm1, [0xba5]
	vcmpgeps k4, zmm0, [0x5dd]
