	vcmpunord_qps xmm5, xmm2, xmm6
	vcmpunord_qps xmm5, xmm6
	vcmpunord_qps xmm5, xmm1, xmm3
	vcmpunord_qps ymm3, ymm0, yword [0x3da]
	vcmpunord_qps ymm3, yword [0x3da]
	vcmpunord_qps ymm1, ymm2, ymm2
	vcmpunord_qps k4, xmm7, xmm1
	vcmpunord_qps k5, xmm3, xmm4
	vcmpunord_qps k4, ymm6, yword [0x522]
	vcmpunord_qps k6, ymm0, ymm7

%ifdef ERROR
	vcmpunord_qps xmm10, xmm8, xmm15
	vcmpunord_qps ymm13, ymm9, ymm9
	vcmpunord_qps k6, xmm13, xmm15
	vcmpunord_qps k1, ymm15, ymm12
%endif
