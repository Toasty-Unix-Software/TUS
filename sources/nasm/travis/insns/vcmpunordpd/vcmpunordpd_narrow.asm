	vcmpunordpd xmm1, xmm1, oword [0x500]
	vcmpunordpd xmm1, oword [0x500]
	vcmpunordpd xmm3, xmm4, xmm0
	vcmpunordpd ymm0, ymm5, ymm4
	vcmpunordpd ymm0, ymm4
	vcmpunordpd ymm1, ymm3, ymm2
	vcmpunordpd k1, xmm2, xmm7
	vcmpunordpd k2, xmm2, xmm1
	vcmpunordpd k7, ymm5, yword [0x9e3]
	vcmpunordpd k4, ymm2, yword [0x6c5]

%ifdef ERROR
	vcmpunordpd xmm11, xmm8, xmm9
	vcmpunordpd ymm14, ymm15, ymm15
	vcmpunordpd k4, xmm8, xmm15
	vcmpunordpd k4, ymm9, ymm15
%endif
