	vcmpfalsepd xmm6, xmm7, xmm6
	vcmpfalsepd xmm6, xmm6
	vcmpfalsepd xmm5, xmm4, xmm6
	vcmpfalsepd ymm4, ymm1, ymm1
	vcmpfalsepd ymm4, ymm1
	vcmpfalsepd ymm4, ymm7, ymm3
	vcmpfalsepd k4, xmm4, xmm7
	vcmpfalsepd k4, xmm3, oword [0x864]
	vcmpfalsepd k2, ymm7, ymm5
	vcmpfalsepd k1, ymm0, yword [0x1f6]

%ifdef ERROR
	vcmpfalsepd xmm10, xmm15, xmm11
	vcmpfalsepd ymm10, ymm13, ymm12
	vcmpfalsepd k1, xmm12, xmm11
	vcmpfalsepd k6, ymm12, ymm8
%endif
