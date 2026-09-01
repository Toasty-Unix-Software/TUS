default rel
	extractps dword [0x687], xmm5, 0x74
	extractps edi, xmm0, 0xb3
	extractps rcx, xmm7, 0x5e
	extractps rbx, xmm2, 0x45
	extractps r15d, xmm11, 0x8b
	extractps r14, xmm9, 0x5c
	extractps dword [eax+1], xmm4, 0xf0
	extractps dword [eax+64], xmm7, 0xe
	extractps [0x57f], xmm7, 0x60
