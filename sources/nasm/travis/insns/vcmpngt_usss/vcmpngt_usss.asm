default rel
	vcmpngt_usss xmm0, xmm1, xmm6
	vcmpngt_usss xmm0, xmm6
	vcmpngt_usss xmm6, xmm6, xmm5
	vcmpngt_usss k6, xmm6, xmm0
	vcmpngt_usss k2, xmm0, dword [0x36d]
	vcmpngt_usss xmm15, xmm9, xmm12
	vcmpngt_usss k7, xmm15, xmm15
	vcmpngt_usss k6{k7}, xmm2, dword [0x74a]
	vcmpngt_usss k5, xmm6, xmm3, {sae}
	vcmpngt_usss xmm3, xmm3, qword [eax+1]
	vcmpngt_usss xmm2, xmm3, qword [eax+64]
	vcmpngt_usss k4, xmm1, dword [eax+1]
	vcmpngt_usss k5, xmm5, dword [eax+64]
	vcmpngt_usss xmm2, xmm7, [0x341]
	vcmpngt_usss k3, xmm6, [0x551]
