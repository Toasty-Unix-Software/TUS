	vunpckhps xmm2, xmm5, oword [0xd20]
	vunpckhps xmm2, oword [0xd20]
	vunpckhps xmm3, xmm1, oword [0xa36]
	vunpckhps ymm7, ymm0, yword [0x898]
	vunpckhps ymm7, yword [0x898]
	vunpckhps ymm3, ymm6, ymm0
	vunpckhps xmm7, xmm3, xmm4
	vunpckhps xmm7, xmm4
	vunpckhps xmm2, xmm5, oword [0xbd3]
	vunpckhps ymm5, ymm1, ymm4
	vunpckhps ymm5, ymm4
	vunpckhps ymm5, ymm3, ymm2

%ifdef ERROR
	vunpckhps xmm15, xmm14, xmm14
	vunpckhps ymm11, ymm14, ymm12
	vunpckhps xmm13, xmm15, xmm11
	vunpckhps ymm12, ymm11, ymm12
	vunpckhps xmm21, xmm21, xmm17
	vunpckhps ymm22, ymm24, ymm21
	vunpckhps xmm25, xmm26, xmm20
	vunpckhps ymm29, ymm28, ymm27
%endif
