default rel
	vcmpngeps xmm7, xmm3, oword [0xcb9]
	vcmpngeps xmm7, oword [0xcb9]
	vcmpngeps xmm3, xmm5, xmm6
	vcmpngeps ymm6, ymm1, ymm1
	vcmpngeps ymm6, ymm1
	vcmpngeps ymm2, ymm6, ymm7
	vcmpngeps k6, xmm3, xmm1
	vcmpngeps k7, xmm4, oword [0x789]
	vcmpngeps k2, ymm2, ymm5
	vcmpngeps k5, ymm1, ymm2
	vcmpngeps xmm11, xmm11, xmm10
	vcmpngeps ymm15, ymm13, ymm10
	vcmpngeps k7, xmm10, xmm13
	vcmpngeps k7, ymm9, ymm11
	vcmpngeps k6{k1}, xmm4, xmm5
	vcmpngeps k1{k2}, ymm1, yword [0x96a]
	vcmpngeps k7{k6}, zmm0, zmm4
	vcmpngeps k7, zmm4, zmm1, {sae}
	vcmpngeps xmm3, xmm5, oword [eax+1]
	vcmpngeps xmm6, xmm3, oword [eax+64]
	vcmpngeps ymm3, ymm4, yword [eax+1]
	vcmpngeps ymm5, ymm2, yword [eax+64]
	vcmpngeps k4, xmm5, oword [eax+1]
	vcmpngeps k5, xmm4, oword [eax+64]
	vcmpngeps k7, ymm3, yword [eax+1]
	vcmpngeps k1, ymm1, yword [eax+64]
	vcmpngeps k5, zmm5, zword [eax+1]
	vcmpngeps k7, zmm7, zword [eax+64]
	vcmpngeps xmm4, xmm3, [0x166]
	vcmpngeps ymm5, ymm5, [0x7bc]
	vcmpngeps k1, xmm7, [0x722]
	vcmpngeps k5, ymm0, [0xd29]
	vcmpngeps k1, zmm0, [0x7ee]
