	vcmpeq_oqpd k3, xmm5, xmm6
	vcmpeq_oqpd k5, xmm2, xmm0
	vcmpeq_oqpd k1, ymm2, ymm0
	vcmpeq_oqpd k6, ymm4, ymm7
	vcmpeq_oqpd k1, zmm5, zmm4
	vcmpeq_oqpd k4, zmm0, zmm4

%ifdef ERROR
	vcmpeq_oqpd k5, xmm11, xmm10
	vcmpeq_oqpd k1, ymm12, ymm12
	vcmpeq_oqpd k6, zmm13, zmm9
	vcmpeq_oqpd k4, xmm20, xmm19
	vcmpeq_oqpd k6, ymm18, ymm16
	vcmpeq_oqpd k1, zmm22, zmm16
%endif
