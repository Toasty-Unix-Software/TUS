default rel
	vcmpngt_ussd xmm5, xmm1, xmm6
	vcmpngt_ussd xmm5, xmm6
	vcmpngt_ussd xmm3, xmm4, qword [0x92e]
	vcmpngt_ussd k6, xmm0, xmm5
	vcmpngt_ussd k7, xmm6, xmm3
	vcmpngt_ussd xmm10, xmm11, xmm10
	vcmpngt_ussd k2, xmm12, xmm8
	vcmpngt_ussd k6{k1}, xmm2, xmm5
	vcmpngt_ussd k2, xmm5, xmm5, {sae}
	vcmpngt_ussd xmm0, xmm4, qword [eax+1]
	vcmpngt_ussd xmm2, xmm5, qword [eax+64]
	vcmpngt_ussd k2, xmm2, qword [eax+1]
	vcmpngt_ussd k1, xmm6, qword [eax+64]
	vcmpngt_ussd xmm6, xmm3, [0xd3f]
	vcmpngt_ussd k2, xmm7, [0xcff]
