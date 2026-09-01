default rel
	vfrczsd xmm3, xmm7
	vfrczsd xmm3
	vfrczsd xmm1, xmm0
	vfrczsd xmm15, xmm10
	vfrczsd xmm1, qword [eax+1]
	vfrczsd xmm3, qword [eax+64]
	vfrczsd xmm2, [0x289]
