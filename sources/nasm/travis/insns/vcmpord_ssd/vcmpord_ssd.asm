default rel
	vcmpord_ssd xmm3, xmm1, xmm6
	vcmpord_ssd xmm3, xmm6
	vcmpord_ssd xmm6, xmm4, xmm0
	vcmpord_ssd k7, xmm2, xmm6
	vcmpord_ssd k2, xmm0, xmm0
	vcmpord_ssd xmm13, xmm8, xmm15
	vcmpord_ssd k2, xmm11, xmm9
	vcmpord_ssd k6{k1}, xmm2, qword [0xbcb]
	vcmpord_ssd k7, xmm5, xmm2, {sae}
	vcmpord_ssd xmm7, xmm7, qword [eax+1]
	vcmpord_ssd xmm5, xmm6, qword [eax+64]
	vcmpord_ssd k3, xmm2, qword [eax+1]
	vcmpord_ssd k5, xmm3, qword [eax+64]
	vcmpord_ssd xmm1, xmm1, [0x745]
	vcmpord_ssd k1, xmm6, [0x5e1]
