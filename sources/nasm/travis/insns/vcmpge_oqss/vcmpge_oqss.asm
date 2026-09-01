default rel
	vcmpge_oqss xmm5, xmm4, xmm6
	vcmpge_oqss xmm5, xmm6
	vcmpge_oqss xmm1, xmm2, xmm7
	vcmpge_oqss k2, xmm1, xmm3
	vcmpge_oqss k7, xmm2, dword [0x125]
	vcmpge_oqss xmm12, xmm11, xmm14
	vcmpge_oqss k3, xmm9, xmm9
	vcmpge_oqss k3{k4}, xmm6, xmm4
	vcmpge_oqss k2, xmm3, xmm0, {sae}
	vcmpge_oqss xmm0, xmm2, qword [eax+1]
	vcmpge_oqss xmm2, xmm5, qword [eax+64]
	vcmpge_oqss k5, xmm3, dword [eax+1]
	vcmpge_oqss k3, xmm1, dword [eax+64]
	vcmpge_oqss xmm2, xmm1, [0xfb0]
	vcmpge_oqss k1, xmm3, [0x55a]
