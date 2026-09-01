default rel
	vpextrd rcx, xmm3, 0xe5
	vpextrd rdi, xmm3, 0xaf
	vpextrd eax, xmm6, 0x5
	vpextrd ebp, xmm2, 0xad
	vpextrd r8, xmm12, 0x1b
	vpextrd r9d, xmm12, 0x6d
	vpextrd dword [eax+1], xmm4, 0x84
	vpextrd dword [eax+64], xmm0, 0xa5
	vpextrd [0x885], xmm6, 0xd4
