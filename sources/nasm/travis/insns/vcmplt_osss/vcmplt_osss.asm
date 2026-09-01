default rel
	vcmplt_osss xmm2, xmm7, xmm0
	vcmplt_osss xmm2, xmm0
	vcmplt_osss xmm6, xmm4, xmm2
	vcmplt_osss k6, xmm5, xmm0
	vcmplt_osss k6, xmm6, xmm7
	vcmplt_osss xmm9, xmm13, xmm9
	vcmplt_osss k2, xmm13, xmm13
	vcmplt_osss k7{k3}, xmm3, xmm3
	vcmplt_osss k1, xmm7, xmm5, {sae}
	vcmplt_osss xmm1, xmm2, qword [eax+1]
	vcmplt_osss xmm5, xmm4, qword [eax+64]
	vcmplt_osss k2, xmm0, dword [eax+1]
	vcmplt_osss k3, xmm2, dword [eax+64]
	vcmplt_osss xmm2, xmm1, [0x302]
	vcmplt_osss k1, xmm6, [0x5f7]
