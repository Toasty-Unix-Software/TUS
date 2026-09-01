default rel
	vcmpunord_qsd xmm0, xmm1, xmm1
	vcmpunord_qsd xmm0, xmm1
	vcmpunord_qsd xmm1, xmm4, xmm2
	vcmpunord_qsd k1, xmm2, xmm4
	vcmpunord_qsd k5, xmm7, qword [0x9ce]
	vcmpunord_qsd xmm9, xmm11, xmm8
	vcmpunord_qsd k7, xmm15, xmm8
	vcmpunord_qsd k5{k7}, xmm7, xmm2
	vcmpunord_qsd k6, xmm1, xmm1, {sae}
	vcmpunord_qsd xmm0, xmm5, qword [eax+1]
	vcmpunord_qsd xmm6, xmm3, qword [eax+64]
	vcmpunord_qsd k7, xmm0, qword [eax+1]
	vcmpunord_qsd k5, xmm3, qword [eax+64]
	vcmpunord_qsd xmm4, xmm3, [0xe62]
	vcmpunord_qsd k2, xmm2, [0x7d5]
