	vpshufd xmm6, xmm5, 0x21
	vpshufd xmm5, xmm0, 0x5c
	vpshufd ymm3, ymm3, 0x76
	vpshufd ymm6, ymm1, 0x35
	vpshufd xmm6, xmm1, 0xc4
	vpshufd xmm3, oword [0x1f9], 0x39
	vpshufd ymm6, ymm7, 0x3f
	vpshufd ymm7, yword [0xad6], 0x6f

%ifdef ERROR
	vpshufd xmm14, xmm14, 0x18
	vpshufd ymm12, ymm15, 0x8e
	vpshufd xmm14, xmm15, 0x7c
	vpshufd ymm8, ymm11, 0x7d
	vpshufd xmm25, xmm30, 0x54
	vpshufd ymm28, ymm29, 0xc9
	vpshufd xmm16, xmm17, 0xf1
	vpshufd ymm22, ymm19, 0x99
%endif
