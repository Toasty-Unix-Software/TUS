	vreducepd xmm3, xmm1, 0xc3
	vreducepd xmm7, xmm5, 0xd5
	vreducepd ymm7, ymm6, 0xdf
	vreducepd ymm0, yword [0xcd2], 0xbe
	vreducepd zmm7, zmm6, 0x90
	vreducepd zmm0, zmm0, 0x18

%ifdef ERROR
	vreducepd xmm12, xmm14, 0x61
	vreducepd ymm10, ymm11, 0x22
	vreducepd zmm12, zmm15, 0xb4
	vreducepd xmm30, xmm18, 0x66
	vreducepd ymm22, ymm26, 0xae
	vreducepd zmm28, zmm16, 0xcf
%endif
