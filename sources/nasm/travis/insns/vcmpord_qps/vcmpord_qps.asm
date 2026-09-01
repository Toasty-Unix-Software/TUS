default rel
	vcmpord_qps xmm2, xmm3, oword [0xf34]
	vcmpord_qps xmm2, oword [0xf34]
	vcmpord_qps xmm1, xmm5, xmm5
	vcmpord_qps ymm4, ymm3, ymm5
	vcmpord_qps ymm4, ymm5
	vcmpord_qps ymm1, ymm1, ymm1
	vcmpord_qps k7, xmm4, xmm5
	vcmpord_qps k6, xmm3, xmm7
	vcmpord_qps k3, ymm7, yword [0x7e0]
	vcmpord_qps k1, ymm5, yword [0x789]
	vcmpord_qps xmm13, xmm11, xmm11
	vcmpord_qps ymm13, ymm14, ymm10
	vcmpord_qps k7, xmm12, xmm13
	vcmpord_qps k1, ymm9, ymm15
	vcmpord_qps k1{k1}, xmm3, xmm0
	vcmpord_qps k3{k7}, ymm2, ymm2
	vcmpord_qps k5{k4}, zmm5, zmm2
	vcmpord_qps k1, zmm1, zmm2, {sae}
	vcmpord_qps xmm3, xmm2, oword [eax+1]
	vcmpord_qps xmm6, xmm0, oword [eax+64]
	vcmpord_qps ymm0, ymm7, yword [eax+1]
	vcmpord_qps ymm1, ymm4, yword [eax+64]
	vcmpord_qps k1, xmm4, oword [eax+1]
	vcmpord_qps k3, xmm7, oword [eax+64]
	vcmpord_qps k1, ymm7, yword [eax+1]
	vcmpord_qps k3, ymm5, yword [eax+64]
	vcmpord_qps k6, zmm5, zword [eax+1]
	vcmpord_qps k1, zmm6, zword [eax+64]
	vcmpord_qps xmm3, xmm7, [0x4b5]
	vcmpord_qps ymm3, ymm1, [0x201]
	vcmpord_qps k1, xmm4, [0xb11]
	vcmpord_qps k3, ymm2, [0xe33]
	vcmpord_qps k7, zmm5, [0x54c]
