default rel
	vcmpnle_usss xmm3, xmm3, xmm7
	vcmpnle_usss xmm3, xmm7
	vcmpnle_usss xmm6, xmm5, xmm2
	vcmpnle_usss k5, xmm0, xmm5
	vcmpnle_usss k6, xmm4, dword [0x62d]
	vcmpnle_usss xmm13, xmm10, xmm13
	vcmpnle_usss k7, xmm9, xmm13
	vcmpnle_usss k2{k5}, xmm0, dword [0x159]
	vcmpnle_usss k5, xmm1, xmm3, {sae}
	vcmpnle_usss xmm5, xmm3, qword [eax+1]
	vcmpnle_usss xmm0, xmm3, qword [eax+64]
	vcmpnle_usss k5, xmm1, dword [eax+1]
	vcmpnle_usss k7, xmm5, dword [eax+64]
	vcmpnle_usss xmm2, xmm3, [0xdd0]
	vcmpnle_usss k6, xmm3, [0x18f]
