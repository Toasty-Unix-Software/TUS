	vcmpgt_oqpd xmm0, xmm5, oword [0x8ff]
	vcmpgt_oqpd xmm0, oword [0x8ff]
	vcmpgt_oqpd xmm1, xmm3, xmm0
	vcmpgt_oqpd ymm3, ymm1, yword [0xa53]
	vcmpgt_oqpd ymm3, yword [0xa53]
	vcmpgt_oqpd ymm3, ymm4, ymm6
	vcmpgt_oqpd k2, xmm7, xmm7
	vcmpgt_oqpd k4, xmm1, xmm1
	vcmpgt_oqpd k5, ymm3, yword [0x83c]
	vcmpgt_oqpd k3, ymm6, ymm7

%ifdef ERROR
	vcmpgt_oqpd xmm11, xmm13, xmm13
	vcmpgt_oqpd ymm9, ymm10, ymm10
	vcmpgt_oqpd k3, xmm15, xmm13
	vcmpgt_oqpd k4, ymm8, ymm11
%endif
