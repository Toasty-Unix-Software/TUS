default rel
	vcmpeqss xmm6, xmm4, xmm1
	vcmpeqss xmm6, xmm1
	vcmpeqss xmm4, xmm4, qword [0xe78]
	vcmpeqss k1, xmm2, dword [0xe3d]
	vcmpeqss k5, xmm7, dword [0x4d6]
	vcmpeqss xmm10, xmm10, xmm14
	vcmpeqss k5, xmm12, xmm13
	vcmpeqss k5{k4}, xmm2, xmm6
	vcmpeqss k4, xmm6, xmm5, {sae}
	vcmpeqss xmm1, xmm5, qword [eax+1]
	vcmpeqss xmm3, xmm6, qword [eax+64]
	vcmpeqss k5, xmm6, dword [eax+1]
	vcmpeqss k3, xmm5, dword [eax+64]
	vcmpeqss xmm3, xmm1, [0x781]
	vcmpeqss k7, xmm1, [0x2cc]
