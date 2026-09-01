default rel
	vucomxss xmm7, xmm0
	vucomxss xmm2, xmm4
	vucomxss xmm11, xmm8
	vucomxss xmm19, xmm17
	vucomxss xmm0, xmm7, {sae}
	vucomxss xmm6, dword [eax+1]
	vucomxss xmm6, dword [eax+64]
	vucomxss xmm1, [0x197]
