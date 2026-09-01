default rel
	vmovss xmm3, xmm4, xmm7
	vmovss xmm3, xmm7
	vmovss xmm2, xmm1, xmm1
	vmovss xmm4, dword [0xb30]
	vmovss xmm6, dword [0xdc6]
	vmovss dword [0x66f], xmm2
	vmovss dword [0x809], xmm3
	vmovss xmm6, dword [0xc04]
	vmovss xmm0, dword [0x7e9]
	vmovss xmm15, xmm10, xmm12
	vmovss xmm8, dword [0x214]
	vmovss dword [0x94a], xmm9
	vmovss xmm9, dword [0xa3a]
	vmovss xmm31, xmm31, xmm25
	vmovss xmm28, dword [0xf7c]
	vmovss dword [0xc59], xmm16
	vmovss xmm18, dword [0x22e]
	vmovss xmm2{k7}, dword [0x11a]
	vmovss dword [0x59e]{k5}, xmm1
	vmovss xmm6{k3}, xmm0, xmm5
	vmovss xmm2, dword [eax+1]
	vmovss xmm2, dword [eax+64]
	vmovss dword [eax+1], xmm6
	vmovss dword [eax+64], xmm3
	vmovss xmm7, dword [eax+1]
	vmovss xmm1, dword [eax+64]
	vmovss dword [eax+1], xmm3
	vmovss dword [eax+64], xmm0
	vmovss xmm1, [0xb75]
	vmovss [0x128], xmm7
	vmovss xmm7, [0x319]
	vmovss [0x7d1], xmm4
