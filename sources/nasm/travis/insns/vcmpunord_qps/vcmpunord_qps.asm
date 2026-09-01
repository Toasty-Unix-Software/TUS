default rel
	vcmpunord_qps xmm5, xmm2, xmm6
	vcmpunord_qps xmm5, xmm6
	vcmpunord_qps xmm5, xmm1, xmm3
	vcmpunord_qps ymm3, ymm0, yword [0x3da]
	vcmpunord_qps ymm3, yword [0x3da]
	vcmpunord_qps ymm1, ymm2, ymm2
	vcmpunord_qps k4, xmm7, xmm1
	vcmpunord_qps k5, xmm3, xmm4
	vcmpunord_qps k4, ymm6, yword [0x522]
	vcmpunord_qps k6, ymm0, ymm7
	vcmpunord_qps xmm10, xmm8, xmm15
	vcmpunord_qps ymm13, ymm9, ymm9
	vcmpunord_qps k6, xmm13, xmm15
	vcmpunord_qps k1, ymm15, ymm12
	vcmpunord_qps k6{k1}, xmm7, oword [0x34f]
	vcmpunord_qps k6{k7}, ymm1, ymm4
	vcmpunord_qps k3{k4}, zmm6, zmm3
	vcmpunord_qps k6, zmm7, zmm5, {sae}
	vcmpunord_qps xmm5, xmm5, oword [eax+1]
	vcmpunord_qps xmm3, xmm6, oword [eax+64]
	vcmpunord_qps ymm7, ymm2, yword [eax+1]
	vcmpunord_qps ymm6, ymm0, yword [eax+64]
	vcmpunord_qps k7, xmm7, oword [eax+1]
	vcmpunord_qps k2, xmm6, oword [eax+64]
	vcmpunord_qps k1, ymm0, yword [eax+1]
	vcmpunord_qps k7, ymm0, yword [eax+64]
	vcmpunord_qps k4, zmm4, zword [eax+1]
	vcmpunord_qps k2, zmm3, zword [eax+64]
	vcmpunord_qps xmm1, xmm2, [0xf12]
	vcmpunord_qps ymm3, ymm6, [0x699]
	vcmpunord_qps k7, xmm4, [0x51e]
	vcmpunord_qps k6, ymm1, [0xefb]
	vcmpunord_qps k5, zmm5, [0xd77]
