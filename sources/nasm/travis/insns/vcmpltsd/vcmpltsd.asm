default rel
	vcmpltsd xmm0, xmm3, xmm6
	vcmpltsd xmm0, xmm6
	vcmpltsd xmm4, xmm3, xmm0
	vcmpltsd k7, xmm3, xmm5
	vcmpltsd k3, xmm7, xmm2
	vcmpltsd xmm13, xmm11, xmm9
	vcmpltsd k6, xmm11, xmm15
	vcmpltsd k6{k1}, xmm1, xmm1
	vcmpltsd k3, xmm2, xmm1, {sae}
	vcmpltsd xmm3, xmm1, qword [eax+1]
	vcmpltsd xmm3, xmm1, qword [eax+64]
	vcmpltsd k7, xmm7, qword [eax+1]
	vcmpltsd k2, xmm6, qword [eax+64]
	vcmpltsd xmm1, xmm4, [0x970]
	vcmpltsd k1, xmm5, [0x1d5]
