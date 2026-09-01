	vcmptrue_usps xmm4, xmm0, xmm3
	vcmptrue_usps xmm4, xmm3
	vcmptrue_usps xmm0, xmm5, xmm6
	vcmptrue_usps ymm6, ymm1, ymm3
	vcmptrue_usps ymm6, ymm3
	vcmptrue_usps ymm4, ymm2, yword [0xa06]
	vcmptrue_usps k1, xmm6, xmm0
	vcmptrue_usps k2, xmm7, xmm1
	vcmptrue_usps k7, ymm7, ymm0
	vcmptrue_usps k6, ymm5, ymm5

%ifdef ERROR
	vcmptrue_usps xmm8, xmm11, xmm15
	vcmptrue_usps ymm11, ymm12, ymm8
	vcmptrue_usps k7, xmm11, xmm12
	vcmptrue_usps k2, ymm11, ymm10
%endif
