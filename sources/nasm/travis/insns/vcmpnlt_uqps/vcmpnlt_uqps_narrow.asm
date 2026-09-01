	vcmpnlt_uqps xmm6, xmm5, oword [0xc56]
	vcmpnlt_uqps xmm6, oword [0xc56]
	vcmpnlt_uqps xmm5, xmm6, xmm2
	vcmpnlt_uqps ymm6, ymm4, ymm5
	vcmpnlt_uqps ymm6, ymm5
	vcmpnlt_uqps ymm5, ymm1, ymm0
	vcmpnlt_uqps k6, xmm1, xmm6
	vcmpnlt_uqps k4, xmm3, xmm7
	vcmpnlt_uqps k7, ymm4, ymm5
	vcmpnlt_uqps k2, ymm3, ymm6

%ifdef ERROR
	vcmpnlt_uqps xmm15, xmm8, xmm8
	vcmpnlt_uqps ymm8, ymm8, ymm9
	vcmpnlt_uqps k3, xmm14, xmm9
	vcmpnlt_uqps k2, ymm11, ymm15
%endif
