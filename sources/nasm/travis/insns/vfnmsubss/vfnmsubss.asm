default rel
	vfnmsubss xmm2, xmm1, xmm1, xmm1
	vfnmsubss xmm2, xmm1, xmm1
	vfnmsubss xmm4, xmm3, xmm1, xmm5
	vfnmsubss xmm5, xmm3, xmm2, dword [0x1f8]
	vfnmsubss xmm5, xmm2, dword [0x1f8]
	vfnmsubss xmm0, xmm3, xmm1, xmm2
	vfnmsubss xmm14, xmm14, xmm15, xmm10
	vfnmsubss xmm10, xmm8, xmm8, xmm12
	vfnmsubss xmm2, xmm0, dword [eax+1], xmm5
	vfnmsubss xmm1, xmm3, dword [eax+64], xmm7
	vfnmsubss xmm7, xmm0, xmm4, dword [eax+1]
	vfnmsubss xmm6, xmm7, xmm4, dword [eax+64]
	vfnmsubss xmm5, xmm5, [0x222], xmm6
	vfnmsubss xmm7, xmm4, xmm6, [0xb79]
