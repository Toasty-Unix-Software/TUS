default rel
	vroundps xmm3, xmm2, 0x94
	vroundps xmm1, xmm4, 0xbe
	vroundps ymm3, yword [0x109], 0x38
	vroundps ymm4, ymm4, 0xf7
	vroundps xmm12, xmm8, 0xc4
	vroundps ymm12, ymm15, 0xe3
	vroundps xmm1, oword [eax+1], 0x5a
	vroundps xmm5, oword [eax+64], 0x37
	vroundps ymm7, yword [eax+1], 0xcc
	vroundps ymm5, yword [eax+64], 0x99
	vroundps xmm2, [0x1ad], 0x6d
	vroundps ymm2, [0xcff], 0x79
