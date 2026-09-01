	vphsubw xmm1, xmm4, xmm7
	vphsubw xmm1, xmm7
	vphsubw xmm2, xmm2, xmm2
	vphsubw ymm3, ymm0, yword [0x81a]
	vphsubw ymm3, yword [0x81a]
	vphsubw ymm5, ymm6, yword [0xb4b]

%ifdef ERROR
	vphsubw xmm8, xmm15, xmm14
	vphsubw ymm11, ymm12, ymm8
%endif
