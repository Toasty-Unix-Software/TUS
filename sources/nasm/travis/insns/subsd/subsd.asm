default rel
	subsd xmm0, xmm6
	subsd xmm5, qword [0x9c5]
	subsd xmm14, xmm13
	subsd xmm0, qword [eax+1]
	subsd xmm6, qword [eax+64]
	subsd xmm7, [0x5b4]
