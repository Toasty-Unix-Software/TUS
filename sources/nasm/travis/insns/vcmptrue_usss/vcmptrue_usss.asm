default rel
	vcmptrue_usss xmm3, xmm1, xmm1
	vcmptrue_usss xmm3, xmm1
	vcmptrue_usss xmm4, xmm7, xmm6
	vcmptrue_usss k1, xmm6, dword [0xc9f]
	vcmptrue_usss k4, xmm6, xmm2
	vcmptrue_usss xmm15, xmm12, xmm8
	vcmptrue_usss k3, xmm14, xmm15
	vcmptrue_usss k1{k2}, xmm5, xmm0
	vcmptrue_usss k5, xmm1, xmm7, {sae}
	vcmptrue_usss xmm2, xmm3, qword [eax+1]
	vcmptrue_usss xmm1, xmm0, qword [eax+64]
	vcmptrue_usss k2, xmm2, dword [eax+1]
	vcmptrue_usss k1, xmm6, dword [eax+64]
	vcmptrue_usss xmm3, xmm0, [0x64c]
	vcmptrue_usss k6, xmm7, [0x867]
