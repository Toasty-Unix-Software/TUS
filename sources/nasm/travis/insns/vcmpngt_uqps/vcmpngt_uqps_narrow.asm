	vcmpngt_uqps xmm4, xmm1, xmm1
	vcmpngt_uqps xmm4, xmm1
	vcmpngt_uqps xmm0, xmm7, oword [0x8be]
	vcmpngt_uqps ymm0, ymm7, yword [0xb40]
	vcmpngt_uqps ymm0, yword [0xb40]
	vcmpngt_uqps ymm6, ymm2, ymm2
	vcmpngt_uqps k2, xmm7, xmm1
	vcmpngt_uqps k2, xmm4, xmm0
	vcmpngt_uqps k6, ymm0, ymm7
	vcmpngt_uqps k2, ymm7, ymm3

%ifdef ERROR
	vcmpngt_uqps xmm14, xmm12, xmm12
	vcmpngt_uqps ymm12, ymm11, ymm10
	vcmpngt_uqps k7, xmm12, xmm10
	vcmpngt_uqps k7, ymm14, ymm15
%endif
