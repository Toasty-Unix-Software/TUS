default rel
	vcmpunord_ssd xmm4, xmm6, qword [0x42f]
	vcmpunord_ssd xmm4, qword [0x42f]
	vcmpunord_ssd xmm6, xmm2, qword [0xb05]
	vcmpunord_ssd k2, xmm3, xmm5
	vcmpunord_ssd k4, xmm4, xmm6
	vcmpunord_ssd xmm11, xmm14, xmm15
	vcmpunord_ssd k4, xmm13, xmm8
	vcmpunord_ssd k6{k3}, xmm1, xmm4
	vcmpunord_ssd k6, xmm0, xmm7, {sae}
	vcmpunord_ssd xmm5, xmm2, qword [eax+1]
	vcmpunord_ssd xmm5, xmm3, qword [eax+64]
	vcmpunord_ssd k4, xmm2, qword [eax+1]
	vcmpunord_ssd k4, xmm3, qword [eax+64]
	vcmpunord_ssd xmm6, xmm5, [0x99e]
	vcmpunord_ssd k7, xmm2, [0xe94]
