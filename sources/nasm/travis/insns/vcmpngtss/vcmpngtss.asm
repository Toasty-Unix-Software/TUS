default rel
	vcmpngtss xmm4, xmm0, xmm3
	vcmpngtss xmm4, xmm3
	vcmpngtss xmm6, xmm3, xmm4
	vcmpngtss k2, xmm2, xmm2
	vcmpngtss k3, xmm6, xmm7
	vcmpngtss xmm15, xmm14, xmm9
	vcmpngtss k5, xmm14, xmm9
	vcmpngtss k1{k1}, xmm3, dword [0xce1]
	vcmpngtss k1, xmm6, xmm5, {sae}
	vcmpngtss xmm0, xmm7, qword [eax+1]
	vcmpngtss xmm7, xmm7, qword [eax+64]
	vcmpngtss k2, xmm7, dword [eax+1]
	vcmpngtss k1, xmm6, dword [eax+64]
	vcmpngtss xmm7, xmm4, [0xd76]
	vcmpngtss k6, xmm7, [0xf30]
