	vmaxps xmm1, xmm0, xmm1
	vmaxps xmm1, xmm1
	vmaxps xmm1, xmm1, xmm0
	vmaxps ymm5, ymm2, ymm0
	vmaxps ymm5, ymm0
	vmaxps ymm3, ymm5, ymm3
	vmaxps xmm0, xmm5, oword [0x5a2]
	vmaxps xmm0, oword [0x5a2]
	vmaxps xmm0, xmm3, xmm3
	vmaxps ymm6, ymm0, ymm3
	vmaxps ymm6, ymm3
	vmaxps ymm2, ymm3, ymm0

%ifdef ERROR
	vmaxps xmm11, xmm14, xmm9
	vmaxps ymm11, ymm8, ymm8
	vmaxps xmm14, xmm12, xmm13
	vmaxps ymm8, ymm12, ymm13
	vmaxps xmm23, xmm17, xmm27
	vmaxps ymm30, ymm20, ymm30
	vmaxps xmm20, xmm26, xmm23
	vmaxps ymm16, ymm29, ymm16
%endif
