default rel
	vcmptrue_ussd xmm2, xmm1, qword [0x65e]
	vcmptrue_ussd xmm2, qword [0x65e]
	vcmptrue_ussd xmm3, xmm7, xmm5
	vcmptrue_ussd k1, xmm7, xmm3
	vcmptrue_ussd k2, xmm7, qword [0x3c4]
	vcmptrue_ussd xmm14, xmm9, xmm14
	vcmptrue_ussd k1, xmm12, xmm10
	vcmptrue_ussd k4{k1}, xmm0, xmm0
	vcmptrue_ussd k5, xmm1, xmm6, {sae}
	vcmptrue_ussd xmm4, xmm5, qword [eax+1]
	vcmptrue_ussd xmm0, xmm0, qword [eax+64]
	vcmptrue_ussd k4, xmm2, qword [eax+1]
	vcmptrue_ussd k7, xmm6, qword [eax+64]
	vcmptrue_ussd xmm2, xmm7, [0xa05]
	vcmptrue_ussd k7, xmm0, [0x5ee]
