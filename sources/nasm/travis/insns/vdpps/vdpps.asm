default rel
	vdpps xmm6, xmm6, xmm2, 0x14
	vdpps xmm6, xmm2, 0x14
	vdpps xmm0, xmm1, xmm6, 0x7e
	vdpps ymm6, ymm6, ymm7, 0xd2
	vdpps ymm6, ymm7, 0xd2
	vdpps ymm0, ymm2, yword [0x5f8], 0x58
	vdpps xmm11, xmm13, xmm15, 0x50
	vdpps ymm10, ymm12, ymm13, 0x5c
	vdpps xmm3, xmm7, oword [eax+1], 0x7c
	vdpps xmm4, xmm2, oword [eax+64], 0x93
	vdpps ymm6, ymm2, yword [eax+1], 0x48
	vdpps ymm6, ymm4, yword [eax+64], 0x6
	vdpps xmm1, xmm1, [0x22b], 0x68
	vdpps ymm0, ymm1, [0xb53], 0x5d
