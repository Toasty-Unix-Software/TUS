	vcmpneq_usps xmm7, xmm3, xmm6
	vcmpneq_usps xmm7, xmm6
	vcmpneq_usps xmm0, xmm4, oword [0x7d0]
	vcmpneq_usps ymm5, ymm2, ymm6
	vcmpneq_usps ymm5, ymm6
	vcmpneq_usps ymm2, ymm2, ymm4
	vcmpneq_usps k7, xmm3, xmm6
	vcmpneq_usps k3, xmm1, oword [0xb1e]
	vcmpneq_usps k3, ymm1, ymm4
	vcmpneq_usps k2, ymm4, ymm0

%ifdef ERROR
	vcmpneq_usps xmm14, xmm12, xmm12
	vcmpneq_usps ymm12, ymm13, ymm14
	vcmpneq_usps k7, xmm14, xmm15
	vcmpneq_usps k1, ymm15, ymm11
%endif
