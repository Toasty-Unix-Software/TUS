default rel
	vroundss xmm4, xmm6, xmm2, 0x2f
	vroundss xmm4, xmm2, 0x2f
	vroundss xmm5, xmm3, xmm2, 0xe9
	vroundss xmm11, xmm13, xmm15, 0xb9
	vroundss xmm4, xmm5, dword [eax+1], 0x4e
	vroundss xmm4, xmm1, dword [eax+64], 0x74
	vroundss xmm1, xmm6, [0x6f0], 0x86
