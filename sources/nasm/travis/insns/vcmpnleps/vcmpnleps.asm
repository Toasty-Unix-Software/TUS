default rel
	vcmpnleps xmm6, xmm7, oword [0xa1d]
	vcmpnleps xmm6, oword [0xa1d]
	vcmpnleps xmm0, xmm6, xmm6
	vcmpnleps ymm3, ymm2, ymm1
	vcmpnleps ymm3, ymm1
	vcmpnleps ymm6, ymm5, ymm0
	vcmpnleps k2, xmm1, xmm6
	vcmpnleps k3, xmm7, xmm5
	vcmpnleps k1, ymm3, yword [0x68c]
	vcmpnleps k6, ymm0, ymm6
	vcmpnleps xmm9, xmm14, xmm12
	vcmpnleps ymm11, ymm8, ymm11
	vcmpnleps k7, xmm12, xmm9
	vcmpnleps k1, ymm13, ymm13
	vcmpnleps k2{k5}, xmm4, oword [0xf4c]
	vcmpnleps k3{k5}, ymm6, ymm4
	vcmpnleps k1{k7}, zmm3, zmm2
	vcmpnleps k1, zmm0, zmm1, {sae}
	vcmpnleps xmm1, xmm1, oword [eax+1]
	vcmpnleps xmm7, xmm3, oword [eax+64]
	vcmpnleps ymm4, ymm3, yword [eax+1]
	vcmpnleps ymm3, ymm0, yword [eax+64]
	vcmpnleps k3, xmm4, oword [eax+1]
	vcmpnleps k5, xmm4, oword [eax+64]
	vcmpnleps k5, ymm2, yword [eax+1]
	vcmpnleps k7, ymm5, yword [eax+64]
	vcmpnleps k3, zmm3, zword [eax+1]
	vcmpnleps k4, zmm4, zword [eax+64]
	vcmpnleps xmm1, xmm0, [0xb80]
	vcmpnleps ymm6, ymm1, [0x104]
	vcmpnleps k4, xmm4, [0xa30]
	vcmpnleps k1, ymm1, [0x6f9]
	vcmpnleps k2, zmm1, [0xe43]
