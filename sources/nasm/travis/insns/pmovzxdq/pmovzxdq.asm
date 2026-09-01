default rel
	pmovzxdq xmm1, xmm7
	pmovzxdq xmm2, xmm6
	pmovzxdq xmm13, xmm9
	pmovzxdq xmm7, qword [eax+1]
	pmovzxdq xmm3, qword [eax+64]
	pmovzxdq xmm5, [0xc1b]
