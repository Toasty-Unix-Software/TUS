	vcmpneqpd xmm1, xmm5, xmm0
	vcmpneqpd xmm1, xmm0
	vcmpneqpd xmm2, xmm1, xmm1
	vcmpneqpd ymm4, ymm3, ymm7
	vcmpneqpd ymm4, ymm7
	vcmpneqpd ymm2, ymm5, ymm2
	vcmpneqpd k4, xmm6, oword [0x9db]
	vcmpneqpd k4, xmm5, xmm5
	vcmpneqpd k1, ymm2, ymm3
	vcmpneqpd k5, ymm6, ymm1

%ifdef ERROR
	vcmpneqpd xmm9, xmm15, xmm15
	vcmpneqpd ymm14, ymm11, ymm9
	vcmpneqpd k6, xmm11, xmm15
	vcmpneqpd k5, ymm9, ymm10
%endif
