default rel
	subss xmm4, xmm6
	subss xmm0, xmm3
	subss xmm14, xmm14
	subss xmm3, dword [eax+1]
	subss xmm4, dword [eax+64]
	subss xmm3, [0x593]
