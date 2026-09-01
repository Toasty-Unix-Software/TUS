default rel
	vfmaddss xmm5, xmm2, xmm4, xmm6
	vfmaddss xmm5, xmm4, xmm6
	vfmaddss xmm6, xmm0, xmm1, xmm7
	vfmaddss xmm6, xmm5, xmm5, dword [0xf1b]
	vfmaddss xmm6, xmm5, dword [0xf1b]
	vfmaddss xmm4, xmm1, xmm4, xmm4
	vfmaddss xmm15, xmm15, xmm10, xmm12
	vfmaddss xmm11, xmm11, xmm11, xmm14
	vfmaddss xmm3, xmm3, dword [eax+1], xmm3
	vfmaddss xmm2, xmm0, dword [eax+64], xmm0
	vfmaddss xmm4, xmm5, xmm5, dword [eax+1]
	vfmaddss xmm1, xmm5, xmm4, dword [eax+64]
	vfmaddss xmm2, xmm3, [0x9fa], xmm0
	vfmaddss xmm2, xmm3, xmm3, [0x7b5]
