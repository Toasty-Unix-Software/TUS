default rel
	vmulsd xmm7, xmm2, xmm2
	vmulsd xmm7, xmm2
	vmulsd xmm1, xmm4, xmm4
	vmulsd xmm7, xmm6, xmm2
	vmulsd xmm7, xmm2
	vmulsd xmm5, xmm6, xmm0
	vmulsd xmm10, xmm10, xmm12
	vmulsd xmm15, xmm13, xmm15
	vmulsd xmm18, xmm16, xmm28
	vmulsd xmm16, xmm24, xmm25
	vmulsd xmm4{k7}, xmm2, xmm0
	vmulsd xmm5{k7}{z}, xmm6, qword [0xd91]
	vmulsd xmm4, xmm7, xmm6, {rd-sae}
	vmulsd xmm2, xmm5, qword [eax+1]
	vmulsd xmm5, xmm0, qword [eax+64]
	vmulsd xmm4, xmm4, qword [eax+1]
	vmulsd xmm3, xmm7, qword [eax+64]
	vmulsd xmm0, xmm1, [0xc45]
	vmulsd xmm7, xmm6, [0x2f9]
