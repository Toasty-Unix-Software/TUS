default rel
	vucomiss xmm3, dword [0xb67]
	vucomiss xmm4, dword [0x92a]
	vucomiss xmm4, xmm2
	vucomiss xmm2, dword [0x20b]
	vucomiss xmm10, xmm8
	vucomiss xmm12, xmm13
	vucomiss xmm17, xmm25
	vucomiss xmm28, xmm24
	vucomiss xmm6, xmm5, {sae}
	vucomiss xmm3, dword [eax+1]
	vucomiss xmm3, dword [eax+64]
	vucomiss xmm7, dword [eax+1]
	vucomiss xmm0, dword [eax+64]
	vucomiss xmm4, [0x407]
	vucomiss xmm0, [0x335]
