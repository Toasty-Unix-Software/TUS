default rel
	vpsrldq xmm0, xmm1, 0x95
	vpsrldq xmm0, 0x95
	vpsrldq xmm3, xmm4, 0x1b
	vpsrldq ymm5, ymm2, 0x5e
	vpsrldq ymm5, 0x5e
	vpsrldq ymm4, ymm2, 0x5b
	vpsrldq xmm3, xmm5, 0x6f
	vpsrldq xmm3, 0x6f
	vpsrldq xmm3, oword [0x3db], 0xbf
	vpsrldq ymm6, yword [0xc03], 0x88
	vpsrldq ymm6, 0x88
	vpsrldq ymm2, ymm1, 0x69
	vpsrldq xmm14, xmm8, 0x83
	vpsrldq ymm8, ymm15, 0x98
	vpsrldq xmm11, xmm11, 0x8
	vpsrldq ymm13, ymm9, 0x35
	vpsrldq xmm31, xmm19, 0xff
	vpsrldq ymm17, ymm20, 0xaf
	vpsrldq xmm29, xmm19, 0x41
	vpsrldq ymm24, ymm27, 0x7d
	vpsrldq xmm6, oword [eax+1], 0xf4
	vpsrldq xmm2, oword [eax+64], 0x96
	vpsrldq ymm4, yword [eax+1], 0x5e
	vpsrldq ymm7, yword [eax+64], 0xf0
	vpsrldq zmm1, zword [eax+1], 0x5b
	vpsrldq zmm0, zword [eax+64], 0xb8
	vpsrldq xmm4, [0xe25], 0x9d
	vpsrldq ymm5, [0xbae], 0xa4
	vpsrldq zmm6, [0x5c3], 0x37
