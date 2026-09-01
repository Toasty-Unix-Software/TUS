default rel
	vcmpneq_oqpd xmm4, xmm6, xmm6
	vcmpneq_oqpd xmm4, xmm6
	vcmpneq_oqpd xmm3, xmm7, xmm7
	vcmpneq_oqpd ymm0, ymm4, ymm7
	vcmpneq_oqpd ymm0, ymm7
	vcmpneq_oqpd ymm1, ymm6, ymm6
	vcmpneq_oqpd k2, xmm2, xmm3
	vcmpneq_oqpd k3, xmm6, xmm1
	vcmpneq_oqpd k6, ymm3, ymm7
	vcmpneq_oqpd k1, ymm1, ymm4
	vcmpneq_oqpd xmm15, xmm13, xmm11
	vcmpneq_oqpd ymm11, ymm12, ymm8
	vcmpneq_oqpd k5, xmm13, xmm10
	vcmpneq_oqpd k4, ymm11, ymm9
	vcmpneq_oqpd k5{k2}, xmm2, xmm6
	vcmpneq_oqpd k7{k6}, ymm4, ymm3
	vcmpneq_oqpd k6{k1}, zmm3, zmm6
	vcmpneq_oqpd k5, zmm6, zmm6, {sae}
	vcmpneq_oqpd xmm4, xmm5, oword [eax+1]
	vcmpneq_oqpd xmm2, xmm7, oword [eax+64]
	vcmpneq_oqpd ymm0, ymm0, yword [eax+1]
	vcmpneq_oqpd ymm7, ymm4, yword [eax+64]
	vcmpneq_oqpd k3, xmm5, oword [eax+1]
	vcmpneq_oqpd k4, xmm3, oword [eax+64]
	vcmpneq_oqpd k5, ymm5, yword [eax+1]
	vcmpneq_oqpd k3, ymm3, yword [eax+64]
	vcmpneq_oqpd k4, zmm4, zword [eax+1]
	vcmpneq_oqpd k4, zmm4, zword [eax+64]
	vcmpneq_oqpd xmm5, xmm4, [0xc9d]
	vcmpneq_oqpd ymm7, ymm2, [0x506]
	vcmpneq_oqpd k2, xmm2, [0xbe2]
	vcmpneq_oqpd k6, ymm1, [0xd95]
	vcmpneq_oqpd k3, zmm2, [0xd09]
