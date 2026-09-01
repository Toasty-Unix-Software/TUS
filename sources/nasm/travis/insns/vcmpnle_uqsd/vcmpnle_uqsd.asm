default rel
	vcmpnle_uqsd xmm0, xmm0, xmm1
	vcmpnle_uqsd xmm0, xmm1
	vcmpnle_uqsd xmm7, xmm5, qword [0xc82]
	vcmpnle_uqsd k4, xmm2, qword [0xc74]
	vcmpnle_uqsd k2, xmm0, xmm3
	vcmpnle_uqsd xmm8, xmm10, xmm13
	vcmpnle_uqsd k7, xmm10, xmm13
	vcmpnle_uqsd k4{k1}, xmm0, xmm0
	vcmpnle_uqsd k2, xmm3, xmm7, {sae}
	vcmpnle_uqsd xmm3, xmm2, qword [eax+1]
	vcmpnle_uqsd xmm3, xmm4, qword [eax+64]
	vcmpnle_uqsd k5, xmm2, qword [eax+1]
	vcmpnle_uqsd k1, xmm6, qword [eax+64]
	vcmpnle_uqsd xmm0, xmm2, [0x2f6]
	vcmpnle_uqsd k2, xmm7, [0xa64]
