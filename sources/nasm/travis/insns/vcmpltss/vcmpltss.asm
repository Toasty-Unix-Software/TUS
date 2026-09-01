default rel
	vcmpltss xmm4, xmm7, qword [0x6ad]
	vcmpltss xmm4, qword [0x6ad]
	vcmpltss xmm2, xmm5, xmm0
	vcmpltss k1, xmm5, xmm3
	vcmpltss k7, xmm7, dword [0x5e3]
	vcmpltss xmm9, xmm12, xmm8
	vcmpltss k1, xmm11, xmm15
	vcmpltss k6{k2}, xmm1, dword [0xcc8]
	vcmpltss k6, xmm3, xmm5, {sae}
	vcmpltss xmm7, xmm6, qword [eax+1]
	vcmpltss xmm5, xmm1, qword [eax+64]
	vcmpltss k3, xmm4, dword [eax+1]
	vcmpltss k3, xmm4, dword [eax+64]
	vcmpltss xmm5, xmm7, [0x91c]
	vcmpltss k5, xmm7, [0x507]
