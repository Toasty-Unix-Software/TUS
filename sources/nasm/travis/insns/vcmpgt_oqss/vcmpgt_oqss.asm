default rel
	vcmpgt_oqss xmm3, xmm3, xmm1
	vcmpgt_oqss xmm3, xmm1
	vcmpgt_oqss xmm0, xmm6, xmm6
	vcmpgt_oqss k3, xmm0, dword [0x106]
	vcmpgt_oqss k6, xmm3, xmm0
	vcmpgt_oqss xmm14, xmm8, xmm14
	vcmpgt_oqss k7, xmm8, xmm14
	vcmpgt_oqss k2{k1}, xmm1, xmm7
	vcmpgt_oqss k2, xmm6, xmm0, {sae}
	vcmpgt_oqss xmm5, xmm2, qword [eax+1]
	vcmpgt_oqss xmm3, xmm6, qword [eax+64]
	vcmpgt_oqss k7, xmm7, dword [eax+1]
	vcmpgt_oqss k2, xmm0, dword [eax+64]
	vcmpgt_oqss xmm3, xmm1, [0xded]
	vcmpgt_oqss k7, xmm3, [0x5be]
