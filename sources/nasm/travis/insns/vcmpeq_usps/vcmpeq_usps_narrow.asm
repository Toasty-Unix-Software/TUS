	vcmpeq_usps xmm6, xmm4, xmm4
	vcmpeq_usps xmm6, xmm4
	vcmpeq_usps xmm4, xmm0, xmm4
	vcmpeq_usps ymm4, ymm7, yword [0xf8d]
	vcmpeq_usps ymm4, yword [0xf8d]
	vcmpeq_usps ymm7, ymm0, yword [0x3fd]
	vcmpeq_usps k6, xmm7, xmm6
	vcmpeq_usps k6, xmm1, xmm6
	vcmpeq_usps k4, ymm5, yword [0xd33]
	vcmpeq_usps k3, ymm2, yword [0xd6c]

%ifdef ERROR
	vcmpeq_usps xmm15, xmm12, xmm10
	vcmpeq_usps ymm10, ymm12, ymm14
	vcmpeq_usps k2, xmm13, xmm13
	vcmpeq_usps k5, ymm15, ymm13
%endif
