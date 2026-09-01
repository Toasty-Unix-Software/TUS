default rel
	pinsrd xmm3, eax, 0xd1
	pinsrd xmm5, dword [0x8be], 0x8d
	pinsrd xmm10, r12d, 0x5e
	pinsrd xmm2, dword [eax+1], 0xa4
	pinsrd xmm1, dword [eax+64], 0x3e
	pinsrd xmm1, [0xd5c], 0x71
