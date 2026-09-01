default rel
	vcmpnltss xmm6, xmm3, xmm7
	vcmpnltss xmm6, xmm7
	vcmpnltss xmm7, xmm0, qword [0x205]
	vcmpnltss k6, xmm1, xmm7
	vcmpnltss k6, xmm7, xmm4
	vcmpnltss xmm14, xmm10, xmm13
	vcmpnltss k4, xmm8, xmm11
	vcmpnltss k6{k2}, xmm5, xmm6
	vcmpnltss k2, xmm5, xmm1, {sae}
	vcmpnltss xmm6, xmm6, qword [eax+1]
	vcmpnltss xmm3, xmm7, qword [eax+64]
	vcmpnltss k4, xmm7, dword [eax+1]
	vcmpnltss k1, xmm0, dword [eax+64]
	vcmpnltss xmm6, xmm3, [0xe00]
	vcmpnltss k4, xmm7, [0xc30]
