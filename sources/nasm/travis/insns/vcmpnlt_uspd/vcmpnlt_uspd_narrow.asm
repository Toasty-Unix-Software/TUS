	vcmpnlt_uspd xmm5, xmm4, xmm1
	vcmpnlt_uspd xmm5, xmm1
	vcmpnlt_uspd xmm5, xmm4, xmm6
	vcmpnlt_uspd ymm7, ymm6, yword [0xe4b]
	vcmpnlt_uspd ymm7, yword [0xe4b]
	vcmpnlt_uspd ymm6, ymm7, ymm2
	vcmpnlt_uspd k4, xmm5, oword [0xdfe]
	vcmpnlt_uspd k7, xmm2, oword [0xc03]
	vcmpnlt_uspd k5, ymm2, ymm3
	vcmpnlt_uspd k2, ymm7, ymm4

%ifdef ERROR
	vcmpnlt_uspd xmm8, xmm12, xmm15
	vcmpnlt_uspd ymm10, ymm12, ymm12
	vcmpnlt_uspd k7, xmm10, xmm15
	vcmpnlt_uspd k7, ymm8, ymm8
%endif
