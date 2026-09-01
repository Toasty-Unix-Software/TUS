default rel
	vreducesd xmm4, xmm5, xmm0, 0x77
	vreducesd xmm4, xmm0, 0x77
	vreducesd xmm6, xmm7, xmm3, 0xcc
	vreducesd xmm13, xmm10, xmm13, 0x3c
	vreducesd xmm31, xmm23, xmm18, 0x29
	vreducesd xmm4{k4}, xmm3, xmm4, 0xb4
	vreducesd xmm5{k4}{z}, xmm1, xmm6, 0xbe
	vreducesd xmm3, xmm6, qword [eax+1], 0x6d
	vreducesd xmm7, xmm3, qword [eax+64], 0x6f
	vreducesd xmm3, xmm5, [0x6c3], 0xe
