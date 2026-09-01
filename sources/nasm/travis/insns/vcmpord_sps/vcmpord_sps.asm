default rel
	vcmpord_sps xmm1, xmm6, xmm2
	vcmpord_sps xmm1, xmm2
	vcmpord_sps xmm1, xmm5, xmm1
	vcmpord_sps ymm7, ymm1, yword [0x1fe]
	vcmpord_sps ymm7, yword [0x1fe]
	vcmpord_sps ymm1, ymm5, yword [0xc6f]
	vcmpord_sps k2, xmm2, oword [0x8e5]
	vcmpord_sps k3, xmm6, xmm4
	vcmpord_sps k3, ymm2, ymm2
	vcmpord_sps k7, ymm3, yword [0x37c]
	vcmpord_sps xmm8, xmm12, xmm11
	vcmpord_sps ymm14, ymm14, ymm15
	vcmpord_sps k4, xmm11, xmm12
	vcmpord_sps k1, ymm11, ymm14
	vcmpord_sps k3{k6}, xmm7, xmm1
	vcmpord_sps k5{k2}, ymm1, yword [0x3a8]
	vcmpord_sps k5{k4}, zmm0, zword [0xba4]
	vcmpord_sps k7, zmm3, zmm7, {sae}
	vcmpord_sps xmm0, xmm5, oword [eax+1]
	vcmpord_sps xmm3, xmm5, oword [eax+64]
	vcmpord_sps ymm3, ymm2, yword [eax+1]
	vcmpord_sps ymm6, ymm5, yword [eax+64]
	vcmpord_sps k5, xmm6, oword [eax+1]
	vcmpord_sps k4, xmm2, oword [eax+64]
	vcmpord_sps k6, ymm1, yword [eax+1]
	vcmpord_sps k4, ymm7, yword [eax+64]
	vcmpord_sps k5, zmm0, zword [eax+1]
	vcmpord_sps k4, zmm4, zword [eax+64]
	vcmpord_sps xmm7, xmm1, [0xa29]
	vcmpord_sps ymm3, ymm1, [0x807]
	vcmpord_sps k2, xmm7, [0x3c7]
	vcmpord_sps k6, ymm7, [0x18c]
	vcmpord_sps k3, zmm4, [0x930]
