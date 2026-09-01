default rel
	vcmpfalse_oqsd xmm1, xmm7, xmm3
	vcmpfalse_oqsd xmm1, xmm3
	vcmpfalse_oqsd xmm1, xmm4, xmm6
	vcmpfalse_oqsd k1, xmm3, xmm4
	vcmpfalse_oqsd k4, xmm5, qword [0x68d]
	vcmpfalse_oqsd xmm9, xmm13, xmm9
	vcmpfalse_oqsd k3, xmm10, xmm8
	vcmpfalse_oqsd k2{k6}, xmm2, xmm1
	vcmpfalse_oqsd k5, xmm0, xmm7, {sae}
	vcmpfalse_oqsd xmm5, xmm1, qword [eax+1]
	vcmpfalse_oqsd xmm2, xmm6, qword [eax+64]
	vcmpfalse_oqsd k1, xmm2, qword [eax+1]
	vcmpfalse_oqsd k4, xmm3, qword [eax+64]
	vcmpfalse_oqsd xmm3, xmm0, [0x9c7]
	vcmpfalse_oqsd k7, xmm0, [0x8c5]
