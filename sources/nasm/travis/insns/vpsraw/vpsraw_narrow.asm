	vpsraw xmm0, xmm3, oword [0xb55]
	vpsraw xmm0, oword [0xb55]
	vpsraw xmm7, xmm3, oword [0xc77]
	vpsraw xmm2, xmm0, 0xc5
	vpsraw xmm2, 0xc5
	vpsraw xmm4, xmm1, 0x1b
	vpsraw ymm3, ymm7, xmm5
	vpsraw ymm3, xmm5
	vpsraw ymm5, ymm5, oword [0xfa2]
	vpsraw ymm5, ymm4, 0xa4
	vpsraw ymm5, 0xa4
	vpsraw ymm7, ymm0, 0xb9

%ifdef ERROR
	vpsraw xmm9, xmm8, xmm14
	vpsraw xmm14, xmm13, 0xfa
	vpsraw ymm13, ymm13, xmm15
	vpsraw ymm12, ymm9, 0x74
	vpsraw xmm17, xmm19, xmm17
	vpsraw xmm16, xmm30, 0xae
	vpsraw ymm21, ymm26, xmm29
	vpsraw ymm16, ymm26, 0x42
%endif
