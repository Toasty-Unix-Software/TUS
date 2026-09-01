	vcmpnge_uqpd xmm2, xmm4, oword [0x87b]
	vcmpnge_uqpd xmm2, oword [0x87b]
	vcmpnge_uqpd xmm6, xmm3, xmm6
	vcmpnge_uqpd ymm2, ymm5, yword [0x73e]
	vcmpnge_uqpd ymm2, yword [0x73e]
	vcmpnge_uqpd ymm0, ymm3, yword [0xaf9]
	vcmpnge_uqpd k2, xmm7, xmm2
	vcmpnge_uqpd k4, xmm1, oword [0x6cc]
	vcmpnge_uqpd k2, ymm2, yword [0xa67]
	vcmpnge_uqpd k7, ymm2, ymm3

%ifdef ERROR
	vcmpnge_uqpd xmm11, xmm12, xmm15
	vcmpnge_uqpd ymm11, ymm11, ymm10
	vcmpnge_uqpd k1, xmm8, xmm10
	vcmpnge_uqpd k1, ymm10, ymm14
%endif
