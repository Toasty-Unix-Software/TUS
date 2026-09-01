	vandnps xmm1, xmm1, oword [0x91f]
	vandnps xmm1, oword [0x91f]
	vandnps xmm2, xmm0, oword [0x5a0]
	vandnps ymm4, ymm7, ymm2
	vandnps ymm4, ymm2
	vandnps ymm6, ymm2, ymm2
	vandnps xmm5, xmm7, xmm6
	vandnps xmm5, xmm6
	vandnps xmm1, xmm4, xmm6
	vandnps ymm4, ymm2, ymm0
	vandnps ymm4, ymm0
	vandnps ymm0, ymm3, ymm3

%ifdef ERROR
	vandnps xmm10, xmm11, xmm13
	vandnps ymm10, ymm12, ymm9
	vandnps xmm13, xmm13, xmm12
	vandnps ymm12, ymm8, ymm11
	vandnps xmm21, xmm29, xmm16
	vandnps ymm31, ymm20, ymm31
	vandnps xmm28, xmm21, xmm17
	vandnps ymm25, ymm30, ymm18
%endif
