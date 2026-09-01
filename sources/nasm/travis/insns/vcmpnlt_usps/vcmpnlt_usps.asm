default rel
	vcmpnlt_usps xmm7, xmm3, xmm3
	vcmpnlt_usps xmm7, xmm3
	vcmpnlt_usps xmm0, xmm0, xmm6
	vcmpnlt_usps ymm6, ymm3, yword [0x168]
	vcmpnlt_usps ymm6, yword [0x168]
	vcmpnlt_usps ymm2, ymm2, yword [0x362]
	vcmpnlt_usps k4, xmm3, oword [0x427]
	vcmpnlt_usps k3, xmm2, oword [0x4dd]
	vcmpnlt_usps k2, ymm1, ymm4
	vcmpnlt_usps k4, ymm2, ymm2
	vcmpnlt_usps xmm12, xmm10, xmm14
	vcmpnlt_usps ymm10, ymm13, ymm9
	vcmpnlt_usps k5, xmm8, xmm15
	vcmpnlt_usps k1, ymm15, ymm8
	vcmpnlt_usps k1{k6}, xmm2, xmm5
	vcmpnlt_usps k5{k5}, ymm5, ymm4
	vcmpnlt_usps k2{k5}, zmm1, zmm5
	vcmpnlt_usps k2, zmm3, zmm1, {sae}
	vcmpnlt_usps xmm0, xmm4, oword [eax+1]
	vcmpnlt_usps xmm2, xmm4, oword [eax+64]
	vcmpnlt_usps ymm6, ymm4, yword [eax+1]
	vcmpnlt_usps ymm3, ymm6, yword [eax+64]
	vcmpnlt_usps k2, xmm6, oword [eax+1]
	vcmpnlt_usps k5, xmm4, oword [eax+64]
	vcmpnlt_usps k2, ymm5, yword [eax+1]
	vcmpnlt_usps k7, ymm2, yword [eax+64]
	vcmpnlt_usps k7, zmm2, zword [eax+1]
	vcmpnlt_usps k1, zmm3, zword [eax+64]
	vcmpnlt_usps xmm7, xmm1, [0xa49]
	vcmpnlt_usps ymm2, ymm3, [0xbde]
	vcmpnlt_usps k5, xmm3, [0x5d7]
	vcmpnlt_usps k4, ymm3, [0xf0c]
	vcmpnlt_usps k7, zmm3, [0x2f0]
