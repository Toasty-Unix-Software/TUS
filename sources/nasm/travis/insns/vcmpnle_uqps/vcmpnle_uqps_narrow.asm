	vcmpnle_uqps xmm7, xmm3, xmm0
	vcmpnle_uqps xmm7, xmm0
	vcmpnle_uqps xmm1, xmm7, xmm1
	vcmpnle_uqps ymm2, ymm2, ymm0
	vcmpnle_uqps ymm2, ymm0
	vcmpnle_uqps ymm3, ymm7, ymm6
	vcmpnle_uqps k3, xmm1, xmm4
	vcmpnle_uqps k4, xmm6, xmm7
	vcmpnle_uqps k2, ymm5, yword [0xce4]
	vcmpnle_uqps k4, ymm7, ymm2

%ifdef ERROR
	vcmpnle_uqps xmm14, xmm14, xmm8
	vcmpnle_uqps ymm13, ymm13, ymm13
	vcmpnle_uqps k5, xmm10, xmm13
	vcmpnle_uqps k7, ymm13, ymm14
%endif
