default rel
	vcmpeq_usss xmm7, xmm2, xmm0
	vcmpeq_usss xmm7, xmm0
	vcmpeq_usss xmm5, xmm3, xmm4
	vcmpeq_usss k2, xmm6, xmm1
	vcmpeq_usss k1, xmm5, xmm2
	vcmpeq_usss xmm9, xmm8, xmm11
	vcmpeq_usss k4, xmm12, xmm9
	vcmpeq_usss k6{k7}, xmm3, dword [0x87a]
	vcmpeq_usss k1, xmm4, xmm6, {sae}
	vcmpeq_usss xmm4, xmm2, qword [eax+1]
	vcmpeq_usss xmm5, xmm7, qword [eax+64]
	vcmpeq_usss k3, xmm1, dword [eax+1]
	vcmpeq_usss k7, xmm6, dword [eax+64]
	vcmpeq_usss xmm6, xmm7, [0x905]
	vcmpeq_usss k3, xmm4, [0x279]
