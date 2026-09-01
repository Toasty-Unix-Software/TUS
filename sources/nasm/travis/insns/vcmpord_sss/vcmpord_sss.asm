default rel
	vcmpord_sss xmm3, xmm4, qword [0xe60]
	vcmpord_sss xmm3, qword [0xe60]
	vcmpord_sss xmm5, xmm5, xmm7
	vcmpord_sss k1, xmm2, xmm7
	vcmpord_sss k3, xmm5, xmm3
	vcmpord_sss xmm9, xmm12, xmm10
	vcmpord_sss k2, xmm15, xmm11
	vcmpord_sss k3{k5}, xmm4, xmm5
	vcmpord_sss k7, xmm2, xmm3, {sae}
	vcmpord_sss xmm3, xmm0, qword [eax+1]
	vcmpord_sss xmm6, xmm5, qword [eax+64]
	vcmpord_sss k6, xmm7, dword [eax+1]
	vcmpord_sss k7, xmm6, dword [eax+64]
	vcmpord_sss xmm7, xmm2, [0xef5]
	vcmpord_sss k7, xmm3, [0x53c]
