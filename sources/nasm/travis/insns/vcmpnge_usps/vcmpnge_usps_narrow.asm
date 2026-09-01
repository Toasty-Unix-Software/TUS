	vcmpnge_usps xmm5, xmm7, xmm6
	vcmpnge_usps xmm5, xmm6
	vcmpnge_usps xmm1, xmm1, oword [0xd1c]
	vcmpnge_usps ymm7, ymm6, ymm6
	vcmpnge_usps ymm7, ymm6
	vcmpnge_usps ymm7, ymm7, ymm2
	vcmpnge_usps k7, xmm1, xmm6
	vcmpnge_usps k2, xmm3, xmm1
	vcmpnge_usps k7, ymm6, ymm5
	vcmpnge_usps k3, ymm0, ymm3

%ifdef ERROR
	vcmpnge_usps xmm9, xmm9, xmm12
	vcmpnge_usps ymm10, ymm8, ymm15
	vcmpnge_usps k5, xmm8, xmm13
	vcmpnge_usps k3, ymm15, ymm15
%endif
