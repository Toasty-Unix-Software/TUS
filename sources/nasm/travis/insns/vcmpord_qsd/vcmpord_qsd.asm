default rel
	vcmpord_qsd xmm0, xmm6, xmm7
	vcmpord_qsd xmm0, xmm7
	vcmpord_qsd xmm3, xmm0, xmm3
	vcmpord_qsd k2, xmm4, xmm2
	vcmpord_qsd k5, xmm5, qword [0x1a1]
	vcmpord_qsd xmm14, xmm12, xmm14
	vcmpord_qsd k1, xmm8, xmm15
	vcmpord_qsd k4{k1}, xmm5, xmm1
	vcmpord_qsd k4, xmm4, xmm3, {sae}
	vcmpord_qsd xmm3, xmm4, qword [eax+1]
	vcmpord_qsd xmm4, xmm0, qword [eax+64]
	vcmpord_qsd k5, xmm4, qword [eax+1]
	vcmpord_qsd k5, xmm1, qword [eax+64]
	vcmpord_qsd xmm1, xmm5, [0x4de]
	vcmpord_qsd k4, xmm4, [0x5a2]
