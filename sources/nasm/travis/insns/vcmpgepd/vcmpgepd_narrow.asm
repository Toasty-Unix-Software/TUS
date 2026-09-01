	vcmpgepd xmm1, xmm0, xmm1
	vcmpgepd xmm1, xmm1
	vcmpgepd xmm7, xmm0, oword [0xa23]
	vcmpgepd ymm7, ymm7, ymm7
	vcmpgepd ymm7, ymm7
	vcmpgepd ymm3, ymm3, yword [0xb07]
	vcmpgepd k4, xmm6, xmm6
	vcmpgepd k4, xmm2, xmm6
	vcmpgepd k1, ymm4, yword [0xc65]
	vcmpgepd k4, ymm1, ymm2

%ifdef ERROR
	vcmpgepd xmm9, xmm10, xmm15
	vcmpgepd ymm11, ymm8, ymm11
	vcmpgepd k5, xmm8, xmm8
	vcmpgepd k5, ymm15, ymm15
%endif
