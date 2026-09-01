default rel
	vfmsubss xmm3, xmm4, xmm4, xmm1
	vfmsubss xmm3, xmm4, xmm1
	vfmsubss xmm2, xmm3, xmm1, xmm6
	vfmsubss xmm7, xmm0, xmm6, xmm5
	vfmsubss xmm7, xmm6, xmm5
	vfmsubss xmm0, xmm5, xmm1, dword [0x155]
	vfmsubss xmm12, xmm9, xmm10, xmm8
	vfmsubss xmm14, xmm9, xmm13, xmm10
	vfmsubss xmm7, xmm1, dword [eax+1], xmm4
	vfmsubss xmm6, xmm7, dword [eax+64], xmm4
	vfmsubss xmm6, xmm1, xmm4, dword [eax+1]
	vfmsubss xmm3, xmm4, xmm2, dword [eax+64]
	vfmsubss xmm7, xmm7, [0x425], xmm0
	vfmsubss xmm1, xmm0, xmm1, [0x806]
