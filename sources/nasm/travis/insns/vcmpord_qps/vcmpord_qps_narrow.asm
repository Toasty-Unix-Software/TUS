	vcmpord_qps xmm2, xmm3, oword [0xf34]
	vcmpord_qps xmm2, oword [0xf34]
	vcmpord_qps xmm1, xmm5, xmm5
	vcmpord_qps ymm4, ymm3, ymm5
	vcmpord_qps ymm4, ymm5
	vcmpord_qps ymm1, ymm1, ymm1
	vcmpord_qps k7, xmm4, xmm5
	vcmpord_qps k6, xmm3, xmm7
	vcmpord_qps k3, ymm7, yword [0x7e0]
	vcmpord_qps k1, ymm5, yword [0x789]

%ifdef ERROR
	vcmpord_qps xmm13, xmm11, xmm11
	vcmpord_qps ymm13, ymm14, ymm10
	vcmpord_qps k7, xmm12, xmm13
	vcmpord_qps k1, ymm9, ymm15
%endif
