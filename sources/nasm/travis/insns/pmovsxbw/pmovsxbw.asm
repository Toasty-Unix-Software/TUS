default rel
	pmovsxbw xmm2, qword [0x693]
	pmovsxbw xmm5, xmm0
	pmovsxbw xmm12, xmm11
	pmovsxbw xmm7, qword [eax+1]
	pmovsxbw xmm2, qword [eax+64]
	pmovsxbw xmm3, [0xb68]
