default rel
	vcmpordsd xmm1, xmm7, qword [0xedf]
	vcmpordsd xmm1, qword [0xedf]
	vcmpordsd xmm3, xmm2, qword [0x1a0]
	vcmpordsd k4, xmm0, qword [0xef1]
	vcmpordsd k4, xmm2, xmm2
	vcmpordsd xmm15, xmm15, xmm10
	vcmpordsd k2, xmm8, xmm9
	vcmpordsd k2{k5}, xmm3, xmm2
	vcmpordsd k3, xmm0, xmm1, {sae}
	vcmpordsd xmm7, xmm1, qword [eax+1]
	vcmpordsd xmm5, xmm7, qword [eax+64]
	vcmpordsd k4, xmm7, qword [eax+1]
	vcmpordsd k7, xmm3, qword [eax+64]
	vcmpordsd xmm7, xmm7, [0x6e0]
	vcmpordsd k7, xmm7, [0xa9d]
