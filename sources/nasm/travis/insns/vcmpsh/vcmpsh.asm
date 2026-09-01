default rel
	vcmpsh k4, xmm5, word [0xc8f], 0xfa
	vcmpsh k4, word [0xc8f], 0xfa
	vcmpsh k3, xmm5, word [0xfd4], 0x7d
	vcmpsh k1, xmm14, xmm9, 0x33
	vcmpsh k2, xmm20, xmm23, 0xba
	vcmpsh k7{k4}, xmm4, xmm6, 0x31
	vcmpsh k3, xmm5, word [eax+1], 0x89
	vcmpsh k4, xmm1, word [eax+64], 0x96
	vcmpsh k5, xmm1, [0x797], 0x18
