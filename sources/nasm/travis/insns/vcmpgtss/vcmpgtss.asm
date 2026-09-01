default rel
	vcmpgtss xmm6, xmm0, qword [0x167]
	vcmpgtss xmm6, qword [0x167]
	vcmpgtss xmm1, xmm6, xmm2
	vcmpgtss k4, xmm5, xmm2
	vcmpgtss k1, xmm7, xmm1
	vcmpgtss xmm13, xmm11, xmm8
	vcmpgtss k5, xmm15, xmm14
	vcmpgtss k5{k3}, xmm5, xmm2
	vcmpgtss k6, xmm3, xmm0, {sae}
	vcmpgtss xmm2, xmm1, qword [eax+1]
	vcmpgtss xmm0, xmm7, qword [eax+64]
	vcmpgtss k3, xmm5, dword [eax+1]
	vcmpgtss k5, xmm1, dword [eax+64]
	vcmpgtss xmm3, xmm2, [0xe09]
	vcmpgtss k3, xmm3, [0x982]
