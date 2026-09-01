default rel
	vcmpnge_uqss xmm7, xmm7, xmm1
	vcmpnge_uqss xmm7, xmm1
	vcmpnge_uqss xmm6, xmm3, qword [0xe32]
	vcmpnge_uqss k4, xmm3, dword [0xc5a]
	vcmpnge_uqss k3, xmm4, dword [0xdde]
	vcmpnge_uqss xmm10, xmm15, xmm8
	vcmpnge_uqss k3, xmm12, xmm9
	vcmpnge_uqss k1{k5}, xmm3, xmm0
	vcmpnge_uqss k2, xmm5, xmm3, {sae}
	vcmpnge_uqss xmm0, xmm4, qword [eax+1]
	vcmpnge_uqss xmm7, xmm7, qword [eax+64]
	vcmpnge_uqss k2, xmm5, dword [eax+1]
	vcmpnge_uqss k2, xmm6, dword [eax+64]
	vcmpnge_uqss xmm4, xmm2, [0xcfa]
	vcmpnge_uqss k7, xmm7, [0x593]
