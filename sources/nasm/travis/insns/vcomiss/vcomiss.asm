default rel
	vcomiss xmm5, xmm7
	vcomiss xmm3, xmm1
	vcomiss xmm5, xmm3
	vcomiss xmm5, dword [0x4a4]
	vcomiss xmm12, xmm11
	vcomiss xmm8, xmm11
	vcomiss xmm31, xmm26
	vcomiss xmm22, xmm24
	vcomiss xmm5, xmm1, {sae}
	vcomiss xmm6, dword [eax+1]
	vcomiss xmm0, dword [eax+64]
	vcomiss xmm7, dword [eax+1]
	vcomiss xmm3, dword [eax+64]
	vcomiss xmm2, [0x1d9]
	vcomiss xmm7, [0xeba]
