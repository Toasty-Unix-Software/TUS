default rel
	vcmpunord_qss xmm4, xmm7, xmm0
	vcmpunord_qss xmm4, xmm0
	vcmpunord_qss xmm7, xmm7, xmm3
	vcmpunord_qss k3, xmm6, xmm5
	vcmpunord_qss k7, xmm4, xmm3
	vcmpunord_qss xmm8, xmm11, xmm14
	vcmpunord_qss k7, xmm13, xmm8
	vcmpunord_qss k3{k5}, xmm7, dword [0xae6]
	vcmpunord_qss k3, xmm3, xmm1, {sae}
	vcmpunord_qss xmm5, xmm7, qword [eax+1]
	vcmpunord_qss xmm3, xmm1, qword [eax+64]
	vcmpunord_qss k4, xmm0, dword [eax+1]
	vcmpunord_qss k7, xmm4, dword [eax+64]
	vcmpunord_qss xmm5, xmm1, [0x66f]
	vcmpunord_qss k6, xmm5, [0x9b8]
