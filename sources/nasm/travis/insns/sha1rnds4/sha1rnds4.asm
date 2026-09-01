default rel
	sha1rnds4 xmm4, xmm3, 0x3e
	sha1rnds4 xmm0, xmm4, 0xe5
	sha1rnds4 xmm12, xmm13, 0x87
	sha1rnds4 xmm2, oword [eax+1], 0x15
	sha1rnds4 xmm2, oword [eax+64], 0x8a
	sha1rnds4 xmm0, [0xac1], 0xd7
