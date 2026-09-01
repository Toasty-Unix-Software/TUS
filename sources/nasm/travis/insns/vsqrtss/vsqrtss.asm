default rel
	vsqrtss xmm4, xmm6, xmm3
	vsqrtss xmm4, xmm3
	vsqrtss xmm4, xmm4, xmm7
	vsqrtss xmm6, xmm7, dword [0x340]
	vsqrtss xmm6, dword [0x340]
	vsqrtss xmm0, xmm0, xmm3
	vsqrtss xmm13, xmm8, xmm9
	vsqrtss xmm10, xmm8, xmm11
	vsqrtss xmm22, xmm27, xmm25
	vsqrtss xmm21, xmm21, xmm22
	vsqrtss xmm2{k4}, xmm7, xmm3
	vsqrtss xmm7{k4}{z}, xmm7, xmm2
	vsqrtss xmm6, xmm6, xmm0, {rz-sae}
	vsqrtss xmm6, xmm2, dword [eax+1]
	vsqrtss xmm0, xmm0, dword [eax+64]
	vsqrtss xmm5, xmm7, dword [eax+1]
	vsqrtss xmm1, xmm6, dword [eax+64]
	vsqrtss xmm6, xmm0, [0xa5e]
	vsqrtss xmm2, xmm4, [0x87f]
