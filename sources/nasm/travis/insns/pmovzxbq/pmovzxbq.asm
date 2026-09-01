default rel
	pmovzxbq xmm3, xmm6
	pmovzxbq xmm1, xmm2
	pmovzxbq xmm13, xmm11
	pmovzxbq xmm0, word [eax+1]
	pmovzxbq xmm4, word [eax+64]
	pmovzxbq xmm6, [0x658]
