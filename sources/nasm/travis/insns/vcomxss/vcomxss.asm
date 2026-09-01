default rel
	vcomxss xmm7, xmm2
	vcomxss xmm6, xmm5
	vcomxss xmm14, xmm8
	vcomxss xmm25, xmm20
	vcomxss xmm3, xmm6, {sae}
	vcomxss xmm6, dword [eax+1]
	vcomxss xmm0, dword [eax+64]
	vcomxss xmm7, [0x784]
