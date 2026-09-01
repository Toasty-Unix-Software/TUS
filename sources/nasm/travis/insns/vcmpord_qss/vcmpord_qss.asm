default rel
	vcmpord_qss xmm7, xmm2, xmm4
	vcmpord_qss xmm7, xmm4
	vcmpord_qss xmm7, xmm6, qword [0xd5c]
	vcmpord_qss k3, xmm5, xmm1
	vcmpord_qss k2, xmm1, dword [0x662]
	vcmpord_qss xmm13, xmm8, xmm9
	vcmpord_qss k1, xmm12, xmm10
	vcmpord_qss k7{k3}, xmm7, xmm5
	vcmpord_qss k5, xmm3, xmm7, {sae}
	vcmpord_qss xmm2, xmm7, qword [eax+1]
	vcmpord_qss xmm6, xmm4, qword [eax+64]
	vcmpord_qss k7, xmm6, dword [eax+1]
	vcmpord_qss k3, xmm5, dword [eax+64]
	vcmpord_qss xmm3, xmm7, [0xab5]
	vcmpord_qss k4, xmm6, [0x2b0]
