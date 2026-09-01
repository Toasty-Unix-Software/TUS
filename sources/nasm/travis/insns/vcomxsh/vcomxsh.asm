default rel
	vcomxsh xmm2, xmm1
	vcomxsh xmm7, xmm5
	vcomxsh xmm10, xmm14
	vcomxsh xmm23, xmm24
	vcomxsh xmm5, xmm2, {sae}
	vcomxsh xmm2, word [eax+1]
	vcomxsh xmm4, word [eax+64]
	vcomxsh xmm0, [0x79a]
