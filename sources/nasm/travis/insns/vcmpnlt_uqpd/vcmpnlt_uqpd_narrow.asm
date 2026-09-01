	vcmpnlt_uqpd xmm4, xmm3, xmm1
	vcmpnlt_uqpd xmm4, xmm1
	vcmpnlt_uqpd xmm7, xmm3, xmm0
	vcmpnlt_uqpd ymm1, ymm6, ymm7
	vcmpnlt_uqpd ymm1, ymm7
	vcmpnlt_uqpd ymm4, ymm7, ymm2
	vcmpnlt_uqpd k6, xmm4, xmm1
	vcmpnlt_uqpd k6, xmm5, xmm2
	vcmpnlt_uqpd k5, ymm2, ymm4
	vcmpnlt_uqpd k4, ymm7, ymm0

%ifdef ERROR
	vcmpnlt_uqpd xmm10, xmm10, xmm9
	vcmpnlt_uqpd ymm12, ymm10, ymm9
	vcmpnlt_uqpd k3, xmm13, xmm12
	vcmpnlt_uqpd k2, ymm12, ymm9
%endif
