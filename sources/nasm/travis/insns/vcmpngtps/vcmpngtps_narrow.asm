	vcmpngtps xmm3, xmm0, xmm7
	vcmpngtps xmm3, xmm7
	vcmpngtps xmm2, xmm7, oword [0x308]
	vcmpngtps ymm6, ymm0, ymm3
	vcmpngtps ymm6, ymm3
	vcmpngtps ymm7, ymm2, ymm4
	vcmpngtps k5, xmm3, xmm2
	vcmpngtps k3, xmm5, oword [0xa6d]
	vcmpngtps k7, ymm4, yword [0x3c9]
	vcmpngtps k6, ymm6, ymm0

%ifdef ERROR
	vcmpngtps xmm9, xmm9, xmm10
	vcmpngtps ymm12, ymm8, ymm13
	vcmpngtps k7, xmm11, xmm10
	vcmpngtps k7, ymm10, ymm15
%endif
