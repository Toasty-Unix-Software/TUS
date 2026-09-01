	vcmpunord_qpd xmm4, xmm6, xmm5
	vcmpunord_qpd xmm4, xmm5
	vcmpunord_qpd xmm5, xmm6, xmm3
	vcmpunord_qpd ymm5, ymm4, yword [0x2d8]
	vcmpunord_qpd ymm5, yword [0x2d8]
	vcmpunord_qpd ymm2, ymm0, yword [0x14d]
	vcmpunord_qpd k1, xmm5, xmm6
	vcmpunord_qpd k2, xmm1, xmm6
	vcmpunord_qpd k2, ymm7, yword [0xd73]
	vcmpunord_qpd k6, ymm7, ymm7

%ifdef ERROR
	vcmpunord_qpd xmm8, xmm8, xmm11
	vcmpunord_qpd ymm8, ymm9, ymm8
	vcmpunord_qpd k7, xmm9, xmm9
	vcmpunord_qpd k2, ymm15, ymm15
%endif
