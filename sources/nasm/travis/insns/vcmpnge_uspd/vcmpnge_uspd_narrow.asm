	vcmpnge_uspd xmm0, xmm1, xmm1
	vcmpnge_uspd xmm0, xmm1
	vcmpnge_uspd xmm6, xmm1, xmm4
	vcmpnge_uspd ymm4, ymm7, ymm2
	vcmpnge_uspd ymm4, ymm2
	vcmpnge_uspd ymm5, ymm4, yword [0xea0]
	vcmpnge_uspd k7, xmm1, xmm1
	vcmpnge_uspd k1, xmm6, oword [0x6bb]
	vcmpnge_uspd k7, ymm7, ymm4
	vcmpnge_uspd k3, ymm5, ymm3

%ifdef ERROR
	vcmpnge_uspd xmm10, xmm12, xmm10
	vcmpnge_uspd ymm13, ymm12, ymm14
	vcmpnge_uspd k3, xmm11, xmm15
	vcmpnge_uspd k5, ymm9, ymm9
%endif
