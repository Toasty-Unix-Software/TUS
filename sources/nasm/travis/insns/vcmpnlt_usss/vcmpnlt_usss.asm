default rel
	vcmpnlt_usss xmm1, xmm2, xmm3
	vcmpnlt_usss xmm1, xmm3
	vcmpnlt_usss xmm3, xmm4, qword [0x6e9]
	vcmpnlt_usss k3, xmm1, dword [0xd9d]
	vcmpnlt_usss k6, xmm2, xmm6
	vcmpnlt_usss xmm14, xmm9, xmm9
	vcmpnlt_usss k6, xmm11, xmm9
	vcmpnlt_usss k2{k5}, xmm5, xmm6
	vcmpnlt_usss k2, xmm2, xmm1, {sae}
	vcmpnlt_usss xmm2, xmm4, qword [eax+1]
	vcmpnlt_usss xmm1, xmm2, qword [eax+64]
	vcmpnlt_usss k4, xmm3, dword [eax+1]
	vcmpnlt_usss k1, xmm7, dword [eax+64]
	vcmpnlt_usss xmm2, xmm1, [0x815]
	vcmpnlt_usss k7, xmm0, [0x410]
