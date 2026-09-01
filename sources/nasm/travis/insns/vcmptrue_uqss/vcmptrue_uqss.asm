default rel
	vcmptrue_uqss xmm5, xmm6, xmm3
	vcmptrue_uqss xmm5, xmm3
	vcmptrue_uqss xmm0, xmm7, xmm1
	vcmptrue_uqss k5, xmm0, xmm7
	vcmptrue_uqss k4, xmm7, xmm0
	vcmptrue_uqss xmm8, xmm15, xmm12
	vcmptrue_uqss k4, xmm12, xmm10
	vcmptrue_uqss k2{k1}, xmm2, xmm0
	vcmptrue_uqss k5, xmm3, xmm5, {sae}
	vcmptrue_uqss xmm3, xmm1, qword [eax+1]
	vcmptrue_uqss xmm6, xmm3, qword [eax+64]
	vcmptrue_uqss k2, xmm1, dword [eax+1]
	vcmptrue_uqss k6, xmm0, dword [eax+64]
	vcmptrue_uqss xmm2, xmm3, [0x885]
	vcmptrue_uqss k5, xmm4, [0x597]
