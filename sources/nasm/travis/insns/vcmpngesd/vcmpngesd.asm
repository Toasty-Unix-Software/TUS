default rel
	vcmpngesd xmm4, xmm1, xmm0
	vcmpngesd xmm4, xmm0
	vcmpngesd xmm3, xmm6, xmm7
	vcmpngesd k7, xmm5, qword [0xaf9]
	vcmpngesd k5, xmm0, xmm1
	vcmpngesd xmm9, xmm10, xmm13
	vcmpngesd k7, xmm10, xmm13
	vcmpngesd k1{k6}, xmm0, xmm4
	vcmpngesd k1, xmm1, xmm7, {sae}
	vcmpngesd xmm6, xmm1, qword [eax+1]
	vcmpngesd xmm4, xmm6, qword [eax+64]
	vcmpngesd k1, xmm6, qword [eax+1]
	vcmpngesd k5, xmm5, qword [eax+64]
	vcmpngesd xmm2, xmm7, [0x764]
	vcmpngesd k3, xmm1, [0xfaf]
