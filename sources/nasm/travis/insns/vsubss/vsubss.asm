default rel
	vsubss xmm5, xmm0, xmm1
	vsubss xmm5, xmm1
	vsubss xmm2, xmm1, xmm0
	vsubss xmm0, xmm6, xmm0
	vsubss xmm0, xmm0
	vsubss xmm6, xmm6, xmm4
	vsubss xmm9, xmm12, xmm12
	vsubss xmm13, xmm9, xmm10
	vsubss xmm19, xmm16, xmm17
	vsubss xmm31, xmm18, xmm24
	vsubss xmm3{k6}, xmm1, xmm5
	vsubss xmm5{k5}{z}, xmm0, dword [0x40c]
	vsubss xmm3, xmm5, xmm4, {ru-sae}
	vsubss xmm3, xmm3, dword [eax+1]
	vsubss xmm4, xmm7, dword [eax+64]
	vsubss xmm1, xmm5, dword [eax+1]
	vsubss xmm0, xmm6, dword [eax+64]
	vsubss xmm4, xmm0, [0x5fa]
	vsubss xmm4, xmm7, [0xae9]
