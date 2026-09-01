	vcmpltpd xmm4, xmm1, xmm2
	vcmpltpd xmm4, xmm2
	vcmpltpd xmm2, xmm6, xmm7
	vcmpltpd ymm3, ymm5, ymm7
	vcmpltpd ymm3, ymm7
	vcmpltpd ymm3, ymm2, ymm3
	vcmpltpd k5, xmm1, oword [0x7ab]
	vcmpltpd k2, xmm2, xmm3
	vcmpltpd k4, ymm0, yword [0x3bb]
	vcmpltpd k7, ymm5, ymm6

%ifdef ERROR
	vcmpltpd xmm9, xmm15, xmm12
	vcmpltpd ymm12, ymm11, ymm11
	vcmpltpd k6, xmm13, xmm14
	vcmpltpd k3, ymm15, ymm14
%endif
