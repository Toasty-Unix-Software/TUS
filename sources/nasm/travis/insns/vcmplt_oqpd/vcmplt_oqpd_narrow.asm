	vcmplt_oqpd xmm5, xmm3, xmm5
	vcmplt_oqpd xmm5, xmm5
	vcmplt_oqpd xmm0, xmm5, xmm7
	vcmplt_oqpd ymm5, ymm2, yword [0xf1b]
	vcmplt_oqpd ymm5, yword [0xf1b]
	vcmplt_oqpd ymm7, ymm5, yword [0x159]
	vcmplt_oqpd k4, xmm6, xmm3
	vcmplt_oqpd k3, xmm3, xmm5
	vcmplt_oqpd k3, ymm4, ymm3
	vcmplt_oqpd k2, ymm7, ymm3

%ifdef ERROR
	vcmplt_oqpd xmm14, xmm15, xmm10
	vcmplt_oqpd ymm10, ymm10, ymm15
	vcmplt_oqpd k7, xmm12, xmm13
	vcmplt_oqpd k3, ymm14, ymm8
%endif
