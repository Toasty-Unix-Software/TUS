default rel
	vcmpunord_sps xmm1, xmm1, xmm6
	vcmpunord_sps xmm1, xmm6
	vcmpunord_sps xmm4, xmm0, xmm1
	vcmpunord_sps ymm5, ymm1, ymm0
	vcmpunord_sps ymm5, ymm0
	vcmpunord_sps ymm3, ymm1, yword [0xa8f]
	vcmpunord_sps k3, xmm7, oword [0xadb]
	vcmpunord_sps k3, xmm7, oword [0xd84]
	vcmpunord_sps k3, ymm6, ymm5
	vcmpunord_sps k5, ymm6, ymm0
	vcmpunord_sps xmm15, xmm15, xmm15
	vcmpunord_sps ymm8, ymm13, ymm12
	vcmpunord_sps k4, xmm9, xmm12
	vcmpunord_sps k2, ymm13, ymm14
	vcmpunord_sps k3{k2}, xmm4, xmm3
	vcmpunord_sps k4{k5}, ymm4, ymm0
	vcmpunord_sps k4{k3}, zmm7, zword [0x7d6]
	vcmpunord_sps k5, zmm5, zmm3, {sae}
	vcmpunord_sps xmm1, xmm0, oword [eax+1]
	vcmpunord_sps xmm1, xmm6, oword [eax+64]
	vcmpunord_sps ymm7, ymm3, yword [eax+1]
	vcmpunord_sps ymm3, ymm3, yword [eax+64]
	vcmpunord_sps k4, xmm3, oword [eax+1]
	vcmpunord_sps k1, xmm3, oword [eax+64]
	vcmpunord_sps k6, ymm2, yword [eax+1]
	vcmpunord_sps k4, ymm2, yword [eax+64]
	vcmpunord_sps k4, zmm5, zword [eax+1]
	vcmpunord_sps k2, zmm1, zword [eax+64]
	vcmpunord_sps xmm1, xmm2, [0x77f]
	vcmpunord_sps ymm7, ymm5, [0x678]
	vcmpunord_sps k3, xmm0, [0x43a]
	vcmpunord_sps k1, ymm2, [0x3cf]
	vcmpunord_sps k6, zmm1, [0xd20]
