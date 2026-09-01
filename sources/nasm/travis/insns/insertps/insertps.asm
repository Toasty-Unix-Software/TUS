default rel
	insertps xmm6, dword [0x39c], 0xce
	insertps xmm5, xmm0, 0x7d
	insertps xmm14, xmm14, 0x48
	insertps xmm5, dword [eax+1], 0xfe
	insertps xmm3, dword [eax+64], 0x7d
	insertps xmm1, [0xa55], 0xbc
