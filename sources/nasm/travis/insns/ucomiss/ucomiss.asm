default rel
	ucomiss xmm1, xmm4
	ucomiss xmm5, dword [0xaa0]
	ucomiss xmm15, xmm14
	ucomiss xmm3, dword [eax+1]
	ucomiss xmm1, dword [eax+64]
	ucomiss xmm5, [0x35d]
