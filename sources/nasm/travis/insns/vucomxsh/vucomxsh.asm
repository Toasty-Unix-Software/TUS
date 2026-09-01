default rel
	vucomxsh xmm5, xmm0
	vucomxsh xmm7, word [0x4c1]
	vucomxsh xmm12, xmm10
	vucomxsh xmm29, xmm31
	vucomxsh xmm2, xmm3, {sae}
	vucomxsh xmm2, word [eax+1]
	vucomxsh xmm1, word [eax+64]
	vucomxsh xmm4, [0xed9]
