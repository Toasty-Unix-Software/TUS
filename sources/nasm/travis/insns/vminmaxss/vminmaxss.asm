default rel
	vminmaxss xmm5, xmm0, dword [0x309], 0xbf
	vminmaxss xmm0, xmm6, xmm4, 0x4d
	vminmaxss xmm15, xmm9, xmm13, 0x30
	vminmaxss xmm25, xmm20, xmm18, 0x89
	vminmaxss xmm2{k6}, xmm0, xmm4, 0x5d
	vminmaxss xmm4{k1}{z}, xmm5, xmm4, 0x8
	vminmaxss xmm3, xmm1, dword [eax+1], 0x2f
	vminmaxss xmm1, xmm3, dword [eax+64], 0x40
	vminmaxss xmm4, xmm2, [0xc61], 0x40
