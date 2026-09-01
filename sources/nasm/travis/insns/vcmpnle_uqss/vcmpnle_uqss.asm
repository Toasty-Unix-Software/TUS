default rel
	vcmpnle_uqss xmm1, xmm0, qword [0xbd1]
	vcmpnle_uqss xmm1, qword [0xbd1]
	vcmpnle_uqss xmm5, xmm2, xmm7
	vcmpnle_uqss k2, xmm0, dword [0xbb5]
	vcmpnle_uqss k5, xmm6, xmm2
	vcmpnle_uqss xmm10, xmm11, xmm12
	vcmpnle_uqss k3, xmm13, xmm11
	vcmpnle_uqss k6{k2}, xmm4, xmm6
	vcmpnle_uqss k5, xmm4, xmm0, {sae}
	vcmpnle_uqss xmm2, xmm1, qword [eax+1]
	vcmpnle_uqss xmm3, xmm5, qword [eax+64]
	vcmpnle_uqss k2, xmm0, dword [eax+1]
	vcmpnle_uqss k4, xmm6, dword [eax+64]
	vcmpnle_uqss xmm5, xmm3, [0xcef]
	vcmpnle_uqss k6, xmm0, [0x471]
