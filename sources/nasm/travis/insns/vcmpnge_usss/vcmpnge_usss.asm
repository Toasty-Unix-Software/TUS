default rel
	vcmpnge_usss xmm4, xmm3, xmm5
	vcmpnge_usss xmm4, xmm5
	vcmpnge_usss xmm1, xmm0, xmm0
	vcmpnge_usss k5, xmm4, xmm7
	vcmpnge_usss k6, xmm3, xmm2
	vcmpnge_usss xmm11, xmm10, xmm12
	vcmpnge_usss k4, xmm15, xmm13
	vcmpnge_usss k6{k4}, xmm1, xmm3
	vcmpnge_usss k5, xmm4, xmm2, {sae}
	vcmpnge_usss xmm0, xmm1, qword [eax+1]
	vcmpnge_usss xmm1, xmm5, qword [eax+64]
	vcmpnge_usss k4, xmm1, dword [eax+1]
	vcmpnge_usss k7, xmm1, dword [eax+64]
	vcmpnge_usss xmm1, xmm4, [0x475]
	vcmpnge_usss k3, xmm4, [0xd8f]
