default rel
	vcvtss2sd xmm3, xmm6, xmm1
	vcvtss2sd xmm3, xmm1
	vcvtss2sd xmm5, xmm1, xmm1
	vcvtss2sd xmm7, xmm1, xmm3
	vcvtss2sd xmm7, xmm3
	vcvtss2sd xmm6, xmm0, xmm6
	vcvtss2sd xmm13, xmm14, xmm8
	vcvtss2sd xmm15, xmm12, xmm13
	vcvtss2sd xmm22, xmm25, xmm31
	vcvtss2sd xmm16, xmm18, xmm23
	vcvtss2sd xmm2{k3}, xmm5, xmm5
	vcvtss2sd xmm5{k2}{z}, xmm7, dword [0xe8e]
	vcvtss2sd xmm5, xmm1, xmm0, {sae}
	vcvtss2sd xmm7, xmm3, dword [eax+1]
	vcvtss2sd xmm7, xmm3, dword [eax+64]
	vcvtss2sd xmm3, xmm3, dword [eax+1]
	vcvtss2sd xmm6, xmm0, dword [eax+64]
	vcvtss2sd xmm7, xmm3, [0x8ef]
	vcvtss2sd xmm3, xmm4, [0xa01]
