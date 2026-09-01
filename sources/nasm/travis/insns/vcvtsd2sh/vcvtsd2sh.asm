default rel
	vcvtsd2sh xmm4, xmm0, xmm1
	vcvtsd2sh xmm4, xmm1
	vcvtsd2sh xmm3, xmm3, xmm0
	vcvtsd2sh xmm8, xmm10, xmm9
	vcvtsd2sh xmm27, xmm22, xmm17
	vcvtsd2sh xmm7{k1}, xmm2, xmm6
	vcvtsd2sh xmm6{k3}{z}, xmm6, xmm4
	vcvtsd2sh xmm6, xmm6, xmm1, {ru-sae}
	vcvtsd2sh xmm6, xmm2, qword [eax+1]
	vcvtsd2sh xmm3, xmm4, qword [eax+64]
	vcvtsd2sh xmm6, xmm3, [0x566]
