default rel
	vcvtsd2ss xmm4, xmm7, xmm0
	vcvtsd2ss xmm4, xmm0
	vcvtsd2ss xmm3, xmm2, xmm5
	vcvtsd2ss xmm2, xmm6, xmm3
	vcvtsd2ss xmm2, xmm3
	vcvtsd2ss xmm0, xmm5, xmm1
	vcvtsd2ss xmm9, xmm12, xmm9
	vcvtsd2ss xmm10, xmm10, xmm8
	vcvtsd2ss xmm19, xmm24, xmm26
	vcvtsd2ss xmm20, xmm31, xmm21
	vcvtsd2ss xmm4{k5}, xmm2, qword [0xba1]
	vcvtsd2ss xmm7{k1}{z}, xmm4, xmm3
	vcvtsd2ss xmm4, xmm5, xmm0, {rz-sae}
	vcvtsd2ss xmm7, xmm2, qword [eax+1]
	vcvtsd2ss xmm5, xmm4, qword [eax+64]
	vcvtsd2ss xmm3, xmm4, qword [eax+1]
	vcvtsd2ss xmm3, xmm4, qword [eax+64]
	vcvtsd2ss xmm5, xmm6, [0x939]
	vcvtsd2ss xmm1, xmm4, [0x897]
