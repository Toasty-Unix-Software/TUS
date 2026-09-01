default rel
	vgetmantss xmm5, xmm7, xmm2, 0x96
	vgetmantss xmm0, xmm2, xmm0, 0x2
	vgetmantss xmm9, xmm15, xmm9, 0x6c
	vgetmantss xmm25, xmm29, xmm28, 0xe3
	vgetmantss xmm4{k3}, xmm1, xmm2, 0x17
	vgetmantss xmm2{k2}{z}, xmm5, dword [0x717], 0xa7
	vgetmantss xmm6, xmm5, dword [eax+1], 0x66
	vgetmantss xmm6, xmm0, dword [eax+64], 0xbf
	vgetmantss xmm5, xmm3, [0xdc6], 0xea
