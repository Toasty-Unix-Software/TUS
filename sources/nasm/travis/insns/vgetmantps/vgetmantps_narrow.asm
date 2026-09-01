	vgetmantps xmm0, xmm1, 0x7d
	vgetmantps xmm2, xmm1, 0x5b
	vgetmantps ymm4, ymm7, 0x36
	vgetmantps ymm4, yword [0xbbe], 0xad
	vgetmantps zmm2, zmm3, 0x13
	vgetmantps zmm6, zmm5, 0x32

%ifdef ERROR
	vgetmantps xmm10, xmm15, 0xe0
	vgetmantps ymm13, ymm12, 0xa9
	vgetmantps zmm10, zmm8, 0xe5
	vgetmantps xmm30, xmm22, 0xf5
	vgetmantps ymm23, ymm18, 0x85
	vgetmantps zmm28, zmm30, 0x89
%endif
