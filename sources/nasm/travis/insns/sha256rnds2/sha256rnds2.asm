default rel
	sha256rnds2 xmm7, xmm7, xmm0
	sha256rnds2 xmm5, xmm0, xmm0
	sha256rnds2 xmm6, xmm2
	sha256rnds2 xmm1, xmm5
	sha256rnds2 xmm15, xmm9, xmm0
	sha256rnds2 xmm11, xmm15
	sha256rnds2 xmm5, oword [eax+1], xmm0
	sha256rnds2 xmm1, oword [eax+64], xmm0
	sha256rnds2 xmm3, oword [eax+1]
	sha256rnds2 xmm0, oword [eax+64]
	sha256rnds2 xmm1, [0x5d8], xmm0
	sha256rnds2 xmm3, [0x96c]
