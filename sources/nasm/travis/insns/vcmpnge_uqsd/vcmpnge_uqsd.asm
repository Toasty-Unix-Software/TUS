default rel
	vcmpnge_uqsd xmm7, xmm0, xmm2
	vcmpnge_uqsd xmm7, xmm2
	vcmpnge_uqsd xmm7, xmm0, xmm3
	vcmpnge_uqsd k3, xmm1, xmm3
	vcmpnge_uqsd k5, xmm4, xmm1
	vcmpnge_uqsd xmm13, xmm8, xmm13
	vcmpnge_uqsd k5, xmm8, xmm11
	vcmpnge_uqsd k7{k4}, xmm4, xmm2
	vcmpnge_uqsd k3, xmm0, xmm4, {sae}
	vcmpnge_uqsd xmm6, xmm1, qword [eax+1]
	vcmpnge_uqsd xmm2, xmm0, qword [eax+64]
	vcmpnge_uqsd k6, xmm0, qword [eax+1]
	vcmpnge_uqsd k2, xmm5, qword [eax+64]
	vcmpnge_uqsd xmm4, xmm0, [0x390]
	vcmpnge_uqsd k3, xmm5, [0x1fb]
