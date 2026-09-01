	vpsllvd xmm7, xmm6, oword [0x2cd]
	vpsllvd xmm7, oword [0x2cd]
	vpsllvd xmm4, xmm1, xmm4
	vpsllvd ymm3, ymm4, yword [0x2a6]
	vpsllvd ymm3, yword [0x2a6]
	vpsllvd ymm2, ymm5, ymm6
	vpsllvd xmm3, xmm5, xmm4
	vpsllvd xmm3, xmm4
	vpsllvd xmm1, xmm0, oword [0xfde]
	vpsllvd ymm5, ymm4, ymm7
	vpsllvd ymm5, ymm7
	vpsllvd ymm3, ymm5, yword [0x448]

%ifdef ERROR
	vpsllvd xmm11, xmm15, xmm14
	vpsllvd ymm12, ymm15, ymm10
	vpsllvd xmm10, xmm9, xmm8
	vpsllvd ymm15, ymm14, ymm12
	vpsllvd xmm18, xmm21, xmm18
	vpsllvd ymm25, ymm25, ymm28
	vpsllvd xmm19, xmm19, xmm26
	vpsllvd ymm24, ymm30, ymm22
%endif
