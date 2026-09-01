default rel
	vcomisd xmm3, xmm0
	vcomisd xmm1, xmm2
	vcomisd xmm5, xmm4
	vcomisd xmm5, xmm5
	vcomisd xmm10, xmm12
	vcomisd xmm11, xmm9
	vcomisd xmm26, xmm18
	vcomisd xmm25, xmm24
	vcomisd xmm5, xmm4, {sae}
	vcomisd xmm3, qword [eax+1]
	vcomisd xmm2, qword [eax+64]
	vcomisd xmm6, qword [eax+1]
	vcomisd xmm5, qword [eax+64]
	vcomisd xmm1, [0x477]
	vcomisd xmm6, [0xbdc]
