default rel
	vcmpngt_uqss xmm2, xmm4, xmm4
	vcmpngt_uqss xmm2, xmm4
	vcmpngt_uqss xmm0, xmm5, qword [0x7f7]
	vcmpngt_uqss k1, xmm2, xmm0
	vcmpngt_uqss k1, xmm2, dword [0x900]
	vcmpngt_uqss xmm8, xmm13, xmm14
	vcmpngt_uqss k1, xmm10, xmm15
	vcmpngt_uqss k6{k6}, xmm6, xmm7
	vcmpngt_uqss k3, xmm0, xmm7, {sae}
	vcmpngt_uqss xmm0, xmm6, qword [eax+1]
	vcmpngt_uqss xmm3, xmm5, qword [eax+64]
	vcmpngt_uqss k4, xmm1, dword [eax+1]
	vcmpngt_uqss k5, xmm6, dword [eax+64]
	vcmpngt_uqss xmm1, xmm6, [0x3e1]
	vcmpngt_uqss k2, xmm1, [0x9fb]
