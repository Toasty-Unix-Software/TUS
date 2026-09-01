default rel
	vcmppd xmm7, xmm1, xmm4, 0xcf
	vcmppd xmm7, xmm4, 0xcf
	vcmppd xmm0, xmm5, xmm1, 0x3c
	vcmppd ymm1, ymm2, ymm2, 0xa8
	vcmppd ymm1, ymm2, 0xa8
	vcmppd ymm5, ymm3, ymm5, 0xd9
	vcmppd k2, xmm1, xmm4, 0x5d
	vcmppd k1, xmm2, oword [0x253], 0x44
	vcmppd k1, ymm1, ymm5, 0x8
	vcmppd k6, ymm5, ymm7, 0xe
	vcmppd xmm14, xmm10, xmm11, 0xf4
	vcmppd ymm8, ymm8, ymm10, 0xb7
	vcmppd k7, xmm15, xmm9, 0xfa
	vcmppd k5, ymm13, ymm15, 0x8a
	vcmppd k7{k6}, xmm7, xmm6, 0x50
	vcmppd k7{k6}, ymm7, ymm4, 0x7a
	vcmppd k1{k2}, zmm4, zmm7, 0x5f
	vcmppd xmm4, xmm5, oword [eax+1], 0x63
	vcmppd xmm7, xmm5, oword [eax+64], 0x53
	vcmppd ymm1, ymm5, yword [eax+1], 0xc7
	vcmppd ymm4, ymm4, yword [eax+64], 0x90
	vcmppd k4, xmm5, oword [eax+1], 0x76
	vcmppd k2, xmm4, oword [eax+64], 0x3a
	vcmppd k2, ymm1, yword [eax+1], 0x3a
	vcmppd k5, ymm7, yword [eax+64], 0xda
	vcmppd k5, zmm5, zword [eax+1], 0x64
	vcmppd k2, zmm5, zword [eax+64], 0x91
	vcmppd xmm2, xmm2, [0xd62], 0x4d
	vcmppd ymm0, ymm1, [0x2f8], 0xd9
	vcmppd k2, xmm0, [0x79e], 0xfc
	vcmppd k1, ymm5, [0xc06], 0x6d
	vcmppd k3, zmm2, [0x559], 0x98
