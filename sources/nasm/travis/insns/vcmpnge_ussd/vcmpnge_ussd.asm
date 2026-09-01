default rel
	vcmpnge_ussd xmm7, xmm7, xmm3
	vcmpnge_ussd xmm7, xmm3
	vcmpnge_ussd xmm5, xmm4, qword [0x3a0]
	vcmpnge_ussd k2, xmm5, qword [0x7e7]
	vcmpnge_ussd k2, xmm6, qword [0x872]
	vcmpnge_ussd xmm9, xmm8, xmm9
	vcmpnge_ussd k1, xmm9, xmm12
	vcmpnge_ussd k2{k1}, xmm7, qword [0xccd]
	vcmpnge_ussd k2, xmm3, xmm4, {sae}
	vcmpnge_ussd xmm5, xmm4, qword [eax+1]
	vcmpnge_ussd xmm4, xmm6, qword [eax+64]
	vcmpnge_ussd k2, xmm1, qword [eax+1]
	vcmpnge_ussd k2, xmm4, qword [eax+64]
	vcmpnge_ussd xmm1, xmm7, [0x841]
	vcmpnge_ussd k4, xmm1, [0x16a]
