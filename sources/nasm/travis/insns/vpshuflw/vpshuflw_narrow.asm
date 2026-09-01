	vpshuflw xmm4, xmm3, 0xd4
	vpshuflw xmm6, xmm1, 0x64
	vpshuflw ymm2, yword [0xd48], 0xd6
	vpshuflw ymm2, yword [0xdcc], 0xe3
	vpshuflw xmm3, xmm1, 0x41
	vpshuflw xmm7, xmm7, 0x26
	vpshuflw ymm1, yword [0xf38], 0xb0
	vpshuflw ymm3, ymm4, 0xf4

%ifdef ERROR
	vpshuflw xmm10, xmm14, 0x71
	vpshuflw ymm12, ymm8, 0xec
	vpshuflw xmm15, xmm13, 0x62
	vpshuflw ymm14, ymm8, 0x3f
	vpshuflw xmm23, xmm25, 0x28
	vpshuflw ymm16, ymm16, 0x31
	vpshuflw xmm16, xmm29, 0x58
	vpshuflw ymm25, ymm17, 0xf0
%endif
