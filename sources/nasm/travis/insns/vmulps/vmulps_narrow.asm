	vmulps xmm5, xmm2, xmm4
	vmulps xmm5, xmm4
	vmulps xmm2, xmm6, oword [0x234]
	vmulps ymm2, ymm1, ymm2
	vmulps ymm2, ymm2
	vmulps ymm4, ymm7, ymm6
	vmulps xmm0, xmm2, xmm0
	vmulps xmm0, xmm0
	vmulps xmm4, xmm1, xmm6
	vmulps ymm3, ymm5, ymm3
	vmulps ymm3, ymm3
	vmulps ymm3, ymm0, ymm2

%ifdef ERROR
	vmulps xmm15, xmm13, xmm10
	vmulps ymm13, ymm14, ymm15
	vmulps xmm12, xmm10, xmm9
	vmulps ymm11, ymm10, ymm9
	vmulps xmm21, xmm29, xmm31
	vmulps ymm18, ymm16, ymm21
	vmulps xmm19, xmm28, xmm20
	vmulps ymm31, ymm18, ymm31
%endif
