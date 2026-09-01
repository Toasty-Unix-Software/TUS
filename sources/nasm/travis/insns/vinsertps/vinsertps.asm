default rel
	vinsertps xmm2, xmm2, xmm4, 0xda
	vinsertps xmm2, xmm4, 0xda
	vinsertps xmm7, xmm7, xmm4, 0x3c
	vinsertps xmm10, xmm12, xmm8, 0x70
	vinsertps xmm16, xmm18, xmm19, 0xde
	vinsertps xmm1, xmm2, dword [eax+1], 0xfd
	vinsertps xmm1, xmm2, dword [eax+64], 0x4
	vinsertps xmm5, xmm7, [0x538], 0xa3
