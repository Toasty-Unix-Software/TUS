default rel
	vcmplt_oqss xmm2, xmm7, xmm1
	vcmplt_oqss xmm2, xmm1
	vcmplt_oqss xmm3, xmm3, qword [0x661]
	vcmplt_oqss k4, xmm7, xmm4
	vcmplt_oqss k6, xmm0, dword [0x90a]
	vcmplt_oqss xmm9, xmm11, xmm11
	vcmplt_oqss k6, xmm8, xmm15
	vcmplt_oqss k6{k3}, xmm7, xmm7
	vcmplt_oqss k7, xmm2, xmm0, {sae}
	vcmplt_oqss xmm7, xmm0, qword [eax+1]
	vcmplt_oqss xmm0, xmm0, qword [eax+64]
	vcmplt_oqss k6, xmm1, dword [eax+1]
	vcmplt_oqss k6, xmm5, dword [eax+64]
	vcmplt_oqss xmm1, xmm3, [0x28b]
	vcmplt_oqss k2, xmm2, [0x35f]
