	vcmpge_oqpd xmm0, xmm5, xmm1
	vcmpge_oqpd xmm0, xmm1
	vcmpge_oqpd xmm3, xmm7, oword [0xf0a]
	vcmpge_oqpd ymm7, ymm7, ymm7
	vcmpge_oqpd ymm7, ymm7
	vcmpge_oqpd ymm7, ymm4, ymm3
	vcmpge_oqpd k4, xmm7, xmm5
	vcmpge_oqpd k1, xmm5, xmm1
	vcmpge_oqpd k1, ymm6, ymm1
	vcmpge_oqpd k7, ymm1, ymm7

%ifdef ERROR
	vcmpge_oqpd xmm13, xmm10, xmm15
	vcmpge_oqpd ymm15, ymm13, ymm15
	vcmpge_oqpd k7, xmm12, xmm12
	vcmpge_oqpd k1, ymm15, ymm9
%endif
