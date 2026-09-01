default rel
	vcmpfalsesd xmm1, xmm3, qword [0x7b2]
	vcmpfalsesd xmm1, qword [0x7b2]
	vcmpfalsesd xmm4, xmm3, xmm5
	vcmpfalsesd k1, xmm7, xmm5
	vcmpfalsesd k5, xmm7, xmm1
	vcmpfalsesd xmm15, xmm10, xmm9
	vcmpfalsesd k1, xmm15, xmm14
	vcmpfalsesd k6{k4}, xmm0, qword [0xc80]
	vcmpfalsesd k2, xmm3, xmm6, {sae}
	vcmpfalsesd xmm4, xmm2, qword [eax+1]
	vcmpfalsesd xmm7, xmm3, qword [eax+64]
	vcmpfalsesd k6, xmm2, qword [eax+1]
	vcmpfalsesd k4, xmm2, qword [eax+64]
	vcmpfalsesd xmm1, xmm6, [0x17c]
	vcmpfalsesd k2, xmm1, [0x826]
