	vgetmantbf16 xmm5, oword [0xe78], 0xb
	vgetmantbf16 xmm0, xmm3, 0xe3
	vgetmantbf16 ymm2, ymm3, 0x3f
	vgetmantbf16 ymm4, ymm5, 0x1
	vgetmantbf16 zmm6, zmm1, 0x50
	vgetmantbf16 zmm3, zmm1, 0x7d

%ifdef ERROR
	vgetmantbf16 xmm10, xmm11, 0xe3
	vgetmantbf16 ymm10, ymm9, 0x9e
	vgetmantbf16 zmm12, zmm8, 0x4
	vgetmantbf16 xmm24, xmm17, 0xf3
	vgetmantbf16 ymm25, ymm21, 0xa8
	vgetmantbf16 zmm25, zmm23, 0x70
%endif
