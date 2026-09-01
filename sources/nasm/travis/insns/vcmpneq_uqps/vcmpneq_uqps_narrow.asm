	vcmpneq_uqps xmm0, xmm1, xmm6
	vcmpneq_uqps xmm0, xmm6
	vcmpneq_uqps xmm3, xmm2, xmm3
	vcmpneq_uqps ymm3, ymm2, ymm6
	vcmpneq_uqps ymm3, ymm6
	vcmpneq_uqps ymm2, ymm2, yword [0xcf2]
	vcmpneq_uqps k1, xmm4, xmm6
	vcmpneq_uqps k5, xmm1, xmm4
	vcmpneq_uqps k2, ymm6, ymm0
	vcmpneq_uqps k6, ymm6, yword [0x1d0]

%ifdef ERROR
	vcmpneq_uqps xmm12, xmm14, xmm13
	vcmpneq_uqps ymm9, ymm11, ymm13
	vcmpneq_uqps k3, xmm12, xmm8
	vcmpneq_uqps k6, ymm13, ymm8
%endif
