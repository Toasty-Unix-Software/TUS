default rel
	vcmptruess xmm6, xmm1, xmm0
	vcmptruess xmm6, xmm0
	vcmptruess xmm2, xmm5, xmm0
	vcmptruess k6, xmm5, xmm5
	vcmptruess k1, xmm6, dword [0xac5]
	vcmptruess xmm12, xmm12, xmm11
	vcmptruess k3, xmm8, xmm13
	vcmptruess k7{k2}, xmm3, xmm5
	vcmptruess k6, xmm1, xmm5, {sae}
	vcmptruess xmm5, xmm7, qword [eax+1]
	vcmptruess xmm5, xmm0, qword [eax+64]
	vcmptruess k3, xmm3, dword [eax+1]
	vcmptruess k3, xmm7, dword [eax+64]
	vcmptruess xmm0, xmm1, [0x9ab]
	vcmptruess k5, xmm2, [0x7a6]
