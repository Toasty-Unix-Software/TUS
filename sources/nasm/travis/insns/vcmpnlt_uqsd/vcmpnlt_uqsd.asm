default rel
	vcmpnlt_uqsd xmm2, xmm1, xmm1
	vcmpnlt_uqsd xmm2, xmm1
	vcmpnlt_uqsd xmm2, xmm7, xmm1
	vcmpnlt_uqsd k1, xmm4, xmm7
	vcmpnlt_uqsd k5, xmm6, xmm0
	vcmpnlt_uqsd xmm11, xmm12, xmm13
	vcmpnlt_uqsd k4, xmm9, xmm9
	vcmpnlt_uqsd k5{k1}, xmm2, qword [0xd92]
	vcmpnlt_uqsd k4, xmm4, xmm5, {sae}
	vcmpnlt_uqsd xmm3, xmm7, qword [eax+1]
	vcmpnlt_uqsd xmm7, xmm0, qword [eax+64]
	vcmpnlt_uqsd k7, xmm5, qword [eax+1]
	vcmpnlt_uqsd k6, xmm5, qword [eax+64]
	vcmpnlt_uqsd xmm1, xmm5, [0xd88]
	vcmpnlt_uqsd k7, xmm4, [0xd0a]
