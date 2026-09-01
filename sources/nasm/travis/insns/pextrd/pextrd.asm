default rel
	pextrd eax, xmm5, 0xcc
	pextrd dword [0x3aa], xmm5, 0x85
	pextrd r14d, xmm14, 0x9
	pextrd dword [eax+1], xmm0, 0x7a
	pextrd dword [eax+64], xmm5, 0x97
	pextrd [0xaf0], xmm3, 0xca
