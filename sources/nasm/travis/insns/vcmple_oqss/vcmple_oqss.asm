default rel
	vcmple_oqss xmm1, xmm0, xmm4
	vcmple_oqss xmm1, xmm4
	vcmple_oqss xmm1, xmm7, qword [0x9ae]
	vcmple_oqss k7, xmm2, xmm3
	vcmple_oqss k5, xmm1, dword [0x41e]
	vcmple_oqss xmm11, xmm14, xmm13
	vcmple_oqss k3, xmm12, xmm14
	vcmple_oqss k7{k6}, xmm0, xmm0
	vcmple_oqss k4, xmm4, xmm3, {sae}
	vcmple_oqss xmm1, xmm5, qword [eax+1]
	vcmple_oqss xmm1, xmm0, qword [eax+64]
	vcmple_oqss k3, xmm7, dword [eax+1]
	vcmple_oqss k3, xmm3, dword [eax+64]
	vcmple_oqss xmm6, xmm7, [0x6a7]
	vcmple_oqss k1, xmm6, [0x2a7]
