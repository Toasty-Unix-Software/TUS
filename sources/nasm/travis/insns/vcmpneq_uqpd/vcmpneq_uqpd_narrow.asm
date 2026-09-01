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

%ifdef ERROR
	vcmpneq_uqpd xmm8, xmm11, xmm9
	vcmpneq_uqpd ymm12, ymm10, ymm14
	vcmpneq_uqpd k2, xmm11, xmm8
	vcmpneq_uqpd k1, ymm8, ymm9
%endif
