default rel
	vcmpeqsd xmm5, xmm4, xmm1
	vcmpeqsd xmm5, xmm1
	vcmpeqsd xmm3, xmm7, qword [0xfef]
	vcmpeqsd k5, xmm3, qword [0xcde]
	vcmpeqsd k4, xmm6, xmm0
	vcmpeqsd xmm8, xmm13, xmm15
	vcmpeqsd k3, xmm8, xmm9
	vcmpeqsd k6{k3}, xmm1, qword [0x12b]
	vcmpeqsd k6, xmm5, xmm7, {sae}
	vcmpeqsd xmm4, xmm4, qword [eax+1]
	vcmpeqsd xmm1, xmm0, qword [eax+64]
	vcmpeqsd k2, xmm6, qword [eax+1]
	vcmpeqsd k1, xmm1, qword [eax+64]
	vcmpeqsd xmm5, xmm0, [0xe81]
	vcmpeqsd k4, xmm0, [0x664]
