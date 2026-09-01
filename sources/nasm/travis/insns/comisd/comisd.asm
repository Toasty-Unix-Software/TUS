default rel
	comisd xmm3, xmm7
	comisd xmm3, xmm4
	comisd xmm15, xmm14
	comisd xmm3, qword [eax+1]
	comisd xmm1, qword [eax+64]
	comisd xmm4, [0x1e8]
