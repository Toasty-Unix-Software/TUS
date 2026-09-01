default rel
	vpshuflw xmm4, xmm3, 0xd4
	vpshuflw xmm6, xmm1, 0x64
	vpshuflw ymm2, yword [0xd48], 0xd6
	vpshuflw ymm2, yword [0xdcc], 0xe3
	vpshuflw xmm3, xmm1, 0x41
	vpshuflw xmm7, xmm7, 0x26
	vpshuflw ymm1, yword [0xf38], 0xb0
	vpshuflw ymm3, ymm4, 0xf4
	vpshuflw xmm10, xmm14, 0x71
	vpshuflw ymm12, ymm8, 0xec
	vpshuflw xmm15, xmm13, 0x62
	vpshuflw ymm14, ymm8, 0x3f
	vpshuflw xmm23, xmm25, 0x28
	vpshuflw ymm16, ymm16, 0x31
	vpshuflw xmm16, xmm29, 0x58
	vpshuflw ymm25, ymm17, 0xf0
	vpshuflw xmm4{k5}, xmm0, 0x88
	vpshuflw ymm1{k3}, ymm7, 0x18
	vpshuflw zmm5{k3}, zmm3, 0x22
	vpshuflw xmm1{k4}{z}, oword [0x343], 0xea
	vpshuflw ymm4{k3}{z}, ymm7, 0x36
	vpshuflw zmm7{k3}{z}, zword [0x405], 0x2b
	vpshuflw xmm1, oword [eax+1], 0xa0
	vpshuflw xmm7, oword [eax+64], 0x13
	vpshuflw ymm6, yword [eax+1], 0x63
	vpshuflw ymm0, yword [eax+64], 0xf9
	vpshuflw xmm7, oword [eax+1], 0x6e
	vpshuflw xmm1, oword [eax+64], 0x55
	vpshuflw ymm3, yword [eax+1], 0x1a
	vpshuflw ymm4, yword [eax+64], 0xfa
	vpshuflw zmm7, zword [eax+1], 0xee
	vpshuflw zmm3, zword [eax+64], 0xc2
	vpshuflw xmm3, [0xff7], 0x68
	vpshuflw ymm0, [0x2a4], 0x78
	vpshuflw xmm6, [0xcae], 0x94
	vpshuflw ymm6, [0x8df], 0x32
	vpshuflw zmm7, [0x1c6], 0x65
