default rel
	vaeskeygenassist xmm7, oword [0x8a8], 0xf4
	vaeskeygenassist xmm2, oword [0xabf], 0xb0
	vaeskeygenassist xmm9, xmm9, 0xb0
	vaeskeygenassist xmm7, oword [eax+1], 0x6a
	vaeskeygenassist xmm1, oword [eax+64], 0x1e
	vaeskeygenassist xmm5, [0xbc9], 0xf5
