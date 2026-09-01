	vpsllw xmm1, xmm3, oword [0xdb9]
	vpsllw xmm1, oword [0xdb9]
	vpsllw xmm3, xmm7, xmm2
	vpsllw xmm5, xmm3, 0x3e
	vpsllw xmm5, 0x3e
	vpsllw xmm5, xmm3, 0x4b
	vpsllw ymm7, ymm6, xmm1
	vpsllw ymm7, xmm1
	vpsllw ymm5, ymm0, oword [0xbf4]
	vpsllw ymm5, ymm2, 0x38
	vpsllw ymm5, 0x38
	vpsllw ymm5, ymm1, 0x21

%ifdef ERROR
	vpsllw xmm10, xmm14, xmm15
	vpsllw xmm11, xmm14, 0x90
	vpsllw ymm14, ymm14, xmm12
	vpsllw ymm10, ymm10, 0x9f
	vpsllw xmm31, xmm19, xmm28
	vpsllw xmm30, xmm25, 0xa0
	vpsllw ymm17, ymm27, xmm16
	vpsllw ymm16, ymm30, 0xf1
%endif
