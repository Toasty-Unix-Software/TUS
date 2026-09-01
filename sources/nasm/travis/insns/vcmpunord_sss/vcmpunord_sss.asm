default rel
	vcmpunord_sss xmm5, xmm5, xmm0
	vcmpunord_sss xmm5, xmm0
	vcmpunord_sss xmm0, xmm7, qword [0xe69]
	vcmpunord_sss k7, xmm3, xmm2
	vcmpunord_sss k2, xmm2, xmm5
	vcmpunord_sss xmm13, xmm11, xmm12
	vcmpunord_sss k1, xmm10, xmm12
	vcmpunord_sss k4{k3}, xmm1, xmm0
	vcmpunord_sss k3, xmm5, xmm0, {sae}
	vcmpunord_sss xmm0, xmm0, qword [eax+1]
	vcmpunord_sss xmm7, xmm0, qword [eax+64]
	vcmpunord_sss k3, xmm6, dword [eax+1]
	vcmpunord_sss k1, xmm4, dword [eax+64]
	vcmpunord_sss xmm4, xmm2, [0x908]
	vcmpunord_sss k1, xmm3, [0xf00]
