default rel
	vcmpeq_oqss k7, xmm5, xmm5
	vcmpeq_oqss k5, xmm5, xmm5
	vcmpeq_oqss k1, xmm11, xmm8
	vcmpeq_oqss k5, xmm27, xmm22
	vcmpeq_oqss k4{k6}, xmm7, xmm0
	vcmpeq_oqss k4, xmm7, xmm7, {sae}
	vcmpeq_oqss k4, xmm6, dword [eax+1]
	vcmpeq_oqss k3, xmm3, dword [eax+64]
	vcmpeq_oqss k2, xmm7, [0x6ab]
