default rel
	vcmpge_oqps xmm6, xmm4, oword [0x569]
	vcmpge_oqps xmm6, oword [0x569]
	vcmpge_oqps xmm4, xmm6, xmm1
	vcmpge_oqps ymm3, ymm4, ymm0
	vcmpge_oqps ymm3, ymm0
	vcmpge_oqps ymm1, ymm3, ymm7
	vcmpge_oqps k2, xmm4, xmm2
	vcmpge_oqps k1, xmm1, xmm0
	vcmpge_oqps k2, ymm3, ymm4
	vcmpge_oqps k4, ymm3, ymm2
	vcmpge_oqps xmm13, xmm10, xmm9
	vcmpge_oqps ymm14, ymm15, ymm11
	vcmpge_oqps k4, xmm11, xmm12
	vcmpge_oqps k6, ymm9, ymm11
	vcmpge_oqps k1{k4}, xmm1, xmm5
	vcmpge_oqps k7{k2}, ymm0, yword [0xd32]
	vcmpge_oqps k3{k4}, zmm5, zmm0
	vcmpge_oqps k4, zmm2, zmm5, {sae}
	vcmpge_oqps xmm6, xmm5, oword [eax+1]
	vcmpge_oqps xmm3, xmm5, oword [eax+64]
	vcmpge_oqps ymm7, ymm1, yword [eax+1]
	vcmpge_oqps ymm1, ymm0, yword [eax+64]
	vcmpge_oqps k2, xmm2, oword [eax+1]
	vcmpge_oqps k2, xmm1, oword [eax+64]
	vcmpge_oqps k7, ymm1, yword [eax+1]
	vcmpge_oqps k5, ymm6, yword [eax+64]
	vcmpge_oqps k6, zmm7, zword [eax+1]
	vcmpge_oqps k3, zmm6, zword [eax+64]
	vcmpge_oqps xmm1, xmm2, [0x8a8]
	vcmpge_oqps ymm0, ymm0, [0x24d]
	vcmpge_oqps k4, xmm6, [0xfc7]
	vcmpge_oqps k2, ymm0, [0xafa]
	vcmpge_oqps k6, zmm7, [0x6eb]
