default rel
	vcmpfalse_oqss xmm5, xmm7, qword [0x5bf]
	vcmpfalse_oqss xmm5, qword [0x5bf]
	vcmpfalse_oqss xmm3, xmm7, qword [0x380]
	vcmpfalse_oqss k6, xmm7, xmm1
	vcmpfalse_oqss k2, xmm3, xmm5
	vcmpfalse_oqss xmm11, xmm14, xmm14
	vcmpfalse_oqss k5, xmm11, xmm11
	vcmpfalse_oqss k3{k4}, xmm2, xmm4
	vcmpfalse_oqss k2, xmm4, xmm6, {sae}
	vcmpfalse_oqss xmm1, xmm2, qword [eax+1]
	vcmpfalse_oqss xmm4, xmm2, qword [eax+64]
	vcmpfalse_oqss k2, xmm6, dword [eax+1]
	vcmpfalse_oqss k3, xmm2, dword [eax+64]
	vcmpfalse_oqss xmm2, xmm6, [0xa8b]
	vcmpfalse_oqss k6, xmm7, [0x61c]
