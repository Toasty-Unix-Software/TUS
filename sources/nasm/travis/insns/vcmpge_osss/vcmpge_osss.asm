default rel
	vcmpge_osss xmm1, xmm3, xmm0
	vcmpge_osss xmm1, xmm0
	vcmpge_osss xmm1, xmm0, xmm1
	vcmpge_osss k1, xmm6, xmm4
	vcmpge_osss k3, xmm7, xmm0
	vcmpge_osss xmm13, xmm11, xmm12
	vcmpge_osss k3, xmm12, xmm15
	vcmpge_osss k1{k4}, xmm5, xmm7
	vcmpge_osss k5, xmm7, xmm0, {sae}
	vcmpge_osss xmm5, xmm4, qword [eax+1]
	vcmpge_osss xmm5, xmm1, qword [eax+64]
	vcmpge_osss k7, xmm7, dword [eax+1]
	vcmpge_osss k1, xmm7, dword [eax+64]
	vcmpge_osss xmm3, xmm3, [0x4a0]
	vcmpge_osss k1, xmm3, [0x488]
