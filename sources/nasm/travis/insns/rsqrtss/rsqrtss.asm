default rel
	rsqrtss xmm4, xmm5
	rsqrtss xmm0, xmm0
	rsqrtss xmm10, xmm8
	rsqrtss xmm7, dword [eax+1]
	rsqrtss xmm0, dword [eax+64]
	rsqrtss xmm6, [0x42f]
