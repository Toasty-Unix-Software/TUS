default rel
	vcmpneq_ussd xmm7, xmm5, xmm6
	vcmpneq_ussd xmm7, xmm6
	vcmpneq_ussd xmm1, xmm1, xmm0
	vcmpneq_ussd k2, xmm5, qword [0x9f5]
	vcmpneq_ussd k6, xmm6, xmm0
	vcmpneq_ussd xmm14, xmm12, xmm8
	vcmpneq_ussd k2, xmm9, xmm15
	vcmpneq_ussd k7{k3}, xmm5, xmm6
	vcmpneq_ussd k2, xmm6, xmm6, {sae}
	vcmpneq_ussd xmm5, xmm1, qword [eax+1]
	vcmpneq_ussd xmm7, xmm0, qword [eax+64]
	vcmpneq_ussd k4, xmm0, qword [eax+1]
	vcmpneq_ussd k1, xmm0, qword [eax+64]
	vcmpneq_ussd xmm4, xmm0, [0xb8c]
	vcmpneq_ussd k1, xmm4, [0x314]
