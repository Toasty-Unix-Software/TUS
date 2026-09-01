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

%ifdef ERROR
	vcmpneq_oqpd xmm15, xmm13, xmm11
	vcmpneq_oqpd ymm11, ymm12, ymm8
	vcmpneq_oqpd k5, xmm13, xmm10
	vcmpneq_oqpd k4, ymm11, ymm9
%endif
