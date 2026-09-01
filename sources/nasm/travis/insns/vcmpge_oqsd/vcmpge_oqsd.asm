default rel
	vcmpge_oqsd xmm7, xmm6, xmm7
	vcmpge_oqsd xmm7, xmm7
	vcmpge_oqsd xmm2, xmm7, xmm1
	vcmpge_oqsd k3, xmm5, xmm5
	vcmpge_oqsd k4, xmm2, xmm0
	vcmpge_oqsd xmm8, xmm11, xmm13
	vcmpge_oqsd k2, xmm15, xmm8
	vcmpge_oqsd k1{k6}, xmm5, xmm0
	vcmpge_oqsd k4, xmm7, xmm4, {sae}
	vcmpge_oqsd xmm4, xmm2, qword [eax+1]
	vcmpge_oqsd xmm7, xmm1, qword [eax+64]
	vcmpge_oqsd k1, xmm3, qword [eax+1]
	vcmpge_oqsd k4, xmm5, qword [eax+64]
	vcmpge_oqsd xmm6, xmm3, [0xe7a]
	vcmpge_oqsd k2, xmm7, [0x515]
