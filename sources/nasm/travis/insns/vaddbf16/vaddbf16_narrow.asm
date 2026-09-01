	vaddbf16 xmm7, xmm1, oword [0x4fd]
	vaddbf16 xmm4, xmm4, oword [0xe03]
	vaddbf16 ymm7, ymm2, yword [0x779]
	vaddbf16 ymm1, ymm6, ymm7
	vaddbf16 zmm1, zmm3, zmm3
	vaddbf16 zmm2, zmm3, zmm0

%ifdef ERROR
	vaddbf16 xmm11, xmm12, xmm8
	vaddbf16 ymm11, ymm12, ymm12
	vaddbf16 zmm15, zmm9, zmm10
	vaddbf16 xmm24, xmm19, xmm24
	vaddbf16 ymm18, ymm29, ymm20
	vaddbf16 zmm17, zmm25, zmm27
%endif
