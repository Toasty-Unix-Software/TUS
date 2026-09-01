default rel
	vcmpneq_uqpd xmm2, xmm5, oword [0x61a]
	vcmpneq_uqpd xmm2, oword [0x61a]
	vcmpneq_uqpd xmm3, xmm6, xmm0
	vcmpneq_uqpd ymm2, ymm3, yword [0x39f]
	vcmpneq_uqpd ymm2, yword [0x39f]
	vcmpneq_uqpd ymm3, ymm3, ymm1
	vcmpneq_uqpd k3, xmm0, xmm7
	vcmpneq_uqpd k7, xmm6, oword [0x30e]
	vcmpneq_uqpd k6, ymm1, ymm0
	vcmpneq_uqpd k2, ymm2, ymm7
	vcmpneq_uqpd xmm8, xmm11, xmm9
	vcmpneq_uqpd ymm12, ymm10, ymm14
	vcmpneq_uqpd k2, xmm11, xmm8
	vcmpneq_uqpd k1, ymm8, ymm9
	vcmpneq_uqpd k5{k5}, xmm0, xmm1
	vcmpneq_uqpd k4{k5}, ymm0, ymm4
	vcmpneq_uqpd k2{k5}, zmm3, zword [0xd9c]
	vcmpneq_uqpd k5, zmm4, zmm5, {sae}
	vcmpneq_uqpd xmm6, xmm5, oword [eax+1]
	vcmpneq_uqpd xmm2, xmm3, oword [eax+64]
	vcmpneq_uqpd ymm5, ymm7, yword [eax+1]
	vcmpneq_uqpd ymm6, ymm1, yword [eax+64]
	vcmpneq_uqpd k6, xmm3, oword [eax+1]
	vcmpneq_uqpd k2, xmm0, oword [eax+64]
	vcmpneq_uqpd k1, ymm5, yword [eax+1]
	vcmpneq_uqpd k6, ymm2, yword [eax+64]
	vcmpneq_uqpd k6, zmm1, zword [eax+1]
	vcmpneq_uqpd k5, zmm7, zword [eax+64]
	vcmpneq_uqpd xmm7, xmm2, [0xb59]
	vcmpneq_uqpd ymm6, ymm6, [0xdcf]
	vcmpneq_uqpd k1, xmm0, [0x774]
	vcmpneq_uqpd k7, ymm2, [0x9cf]
	vcmpneq_uqpd k1, zmm1, [0x50e]
