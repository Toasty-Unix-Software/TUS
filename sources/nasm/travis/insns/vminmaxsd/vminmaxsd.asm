default rel
	vminmaxsd xmm0, xmm3, qword [0xf58], 0x91
	vminmaxsd xmm0, xmm6, qword [0x41e], 0xbf
	vminmaxsd xmm8, xmm9, xmm9, 0x39
	vminmaxsd xmm19, xmm23, xmm30, 0x68
	vminmaxsd xmm7{k1}, xmm6, qword [0x390], 0x5f
	vminmaxsd xmm4{k7}{z}, xmm3, xmm2, 0x44
	vminmaxsd xmm6, xmm6, qword [eax+1], 0x56
	vminmaxsd xmm6, xmm7, qword [eax+64], 0xd4
	vminmaxsd xmm0, xmm7, [0x164], 0xca
