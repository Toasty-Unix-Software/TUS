	vorps xmm2, xmm2, xmm5
	vorps xmm2, xmm5
	vorps xmm5, xmm1, xmm4
	vorps ymm2, ymm2, ymm2
	vorps ymm2, ymm2
	vorps ymm2, ymm1, ymm4
	vorps xmm6, xmm3, oword [0x8a7]
	vorps xmm6, oword [0x8a7]
	vorps xmm2, xmm4, oword [0x2c1]
	vorps ymm0, ymm5, ymm6
	vorps ymm0, ymm6
	vorps ymm1, ymm3, ymm1

%ifdef ERROR
	vorps xmm13, xmm14, xmm8
	vorps ymm14, ymm13, ymm12
	vorps xmm14, xmm10, xmm15
	vorps ymm13, ymm15, ymm8
	vorps xmm27, xmm22, xmm17
	vorps ymm18, ymm30, ymm22
	vorps xmm21, xmm19, xmm20
	vorps ymm29, ymm23, ymm25
%endif
