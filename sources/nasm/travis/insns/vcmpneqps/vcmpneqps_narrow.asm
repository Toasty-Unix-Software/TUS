	vcmpneqps xmm5, xmm1, xmm3
	vcmpneqps xmm5, xmm3
	vcmpneqps xmm4, xmm0, xmm5
	vcmpneqps ymm2, ymm3, ymm0
	vcmpneqps ymm2, ymm0
	vcmpneqps ymm0, ymm4, yword [0xeac]
	vcmpneqps k4, xmm0, xmm5
	vcmpneqps k7, xmm3, xmm4
	vcmpneqps k7, ymm0, ymm2
	vcmpneqps k5, ymm0, yword [0xa86]

%ifdef ERROR
	vcmpneqps xmm12, xmm15, xmm13
	vcmpneqps ymm13, ymm10, ymm12
	vcmpneqps k2, xmm12, xmm9
	vcmpneqps k7, ymm10, ymm15
%endif
