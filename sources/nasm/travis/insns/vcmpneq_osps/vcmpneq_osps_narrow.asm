	vcmpneq_osps xmm6, xmm6, xmm0
	vcmpneq_osps xmm6, xmm0
	vcmpneq_osps xmm7, xmm0, oword [0x53a]
	vcmpneq_osps ymm5, ymm0, yword [0xa4d]
	vcmpneq_osps ymm5, yword [0xa4d]
	vcmpneq_osps ymm1, ymm2, yword [0xcf3]
	vcmpneq_osps k7, xmm1, xmm3
	vcmpneq_osps k4, xmm4, xmm5
	vcmpneq_osps k7, ymm4, ymm5
	vcmpneq_osps k2, ymm1, yword [0xccf]

%ifdef ERROR
	vcmpneq_osps xmm14, xmm15, xmm14
	vcmpneq_osps ymm8, ymm14, ymm12
	vcmpneq_osps k4, xmm8, xmm9
	vcmpneq_osps k6, ymm10, ymm9
%endif
