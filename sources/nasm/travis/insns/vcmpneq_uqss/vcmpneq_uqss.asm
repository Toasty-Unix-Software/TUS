default rel
	vcmpneq_uqss xmm3, xmm1, xmm1
	vcmpneq_uqss xmm3, xmm1
	vcmpneq_uqss xmm3, xmm6, xmm7
	vcmpneq_uqss k7, xmm1, xmm0
	vcmpneq_uqss k4, xmm5, dword [0xd38]
	vcmpneq_uqss xmm10, xmm9, xmm12
	vcmpneq_uqss k3, xmm9, xmm10
	vcmpneq_uqss k4{k5}, xmm3, xmm1
	vcmpneq_uqss k3, xmm0, xmm2, {sae}
	vcmpneq_uqss xmm1, xmm6, qword [eax+1]
	vcmpneq_uqss xmm0, xmm6, qword [eax+64]
	vcmpneq_uqss k5, xmm0, dword [eax+1]
	vcmpneq_uqss k3, xmm0, dword [eax+64]
	vcmpneq_uqss xmm2, xmm6, [0x614]
	vcmpneq_uqss k1, xmm0, [0x50b]
