	vcmpeq_uqps xmm1, xmm0, oword [0x36e]
	vcmpeq_uqps xmm1, oword [0x36e]
	vcmpeq_uqps xmm0, xmm7, xmm0
	vcmpeq_uqps ymm2, ymm3, ymm3
	vcmpeq_uqps ymm2, ymm3
	vcmpeq_uqps ymm0, ymm0, ymm6
	vcmpeq_uqps k1, xmm3, oword [0x655]
	vcmpeq_uqps k4, xmm6, oword [0xfae]
	vcmpeq_uqps k1, ymm0, ymm6
	vcmpeq_uqps k1, ymm0, yword [0xb80]

%ifdef ERROR
	vcmpeq_uqps xmm12, xmm14, xmm14
	vcmpeq_uqps ymm12, ymm9, ymm12
	vcmpeq_uqps k3, xmm14, xmm8
	vcmpeq_uqps k7, ymm14, ymm12
%endif
