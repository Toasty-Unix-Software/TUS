default rel
	vcmpnlt_ussd xmm3, xmm1, xmm6
	vcmpnlt_ussd xmm3, xmm6
	vcmpnlt_ussd xmm3, xmm4, xmm3
	vcmpnlt_ussd k4, xmm0, qword [0x6ac]
	vcmpnlt_ussd k4, xmm4, xmm7
	vcmpnlt_ussd xmm10, xmm14, xmm10
	vcmpnlt_ussd k4, xmm15, xmm13
	vcmpnlt_ussd k6{k4}, xmm4, xmm3
	vcmpnlt_ussd k4, xmm6, xmm3, {sae}
	vcmpnlt_ussd xmm4, xmm6, qword [eax+1]
	vcmpnlt_ussd xmm6, xmm0, qword [eax+64]
	vcmpnlt_ussd k4, xmm6, qword [eax+1]
	vcmpnlt_ussd k3, xmm3, qword [eax+64]
	vcmpnlt_ussd xmm5, xmm4, [0x7e4]
	vcmpnlt_ussd k1, xmm5, [0x112]
