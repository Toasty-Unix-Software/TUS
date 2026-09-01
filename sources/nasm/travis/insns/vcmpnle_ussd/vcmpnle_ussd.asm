default rel
	vcmpnle_ussd xmm3, xmm7, xmm4
	vcmpnle_ussd xmm3, xmm4
	vcmpnle_ussd xmm0, xmm3, xmm0
	vcmpnle_ussd k6, xmm2, xmm6
	vcmpnle_ussd k7, xmm4, xmm6
	vcmpnle_ussd xmm14, xmm8, xmm10
	vcmpnle_ussd k1, xmm15, xmm12
	vcmpnle_ussd k4{k3}, xmm4, xmm3
	vcmpnle_ussd k1, xmm4, xmm2, {sae}
	vcmpnle_ussd xmm0, xmm2, qword [eax+1]
	vcmpnle_ussd xmm1, xmm4, qword [eax+64]
	vcmpnle_ussd k6, xmm3, qword [eax+1]
	vcmpnle_ussd k2, xmm7, qword [eax+64]
	vcmpnle_ussd xmm1, xmm1, [0xf52]
	vcmpnle_ussd k2, xmm3, [0x81e]
