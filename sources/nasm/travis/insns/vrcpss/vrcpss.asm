default rel
	vrcpss xmm2, xmm2, xmm6
	vrcpss xmm2, xmm6
	vrcpss xmm2, xmm1, dword [0x97e]
	vrcpss xmm8, xmm15, xmm12
	vrcpss xmm4, xmm5, dword [eax+1]
	vrcpss xmm1, xmm1, dword [eax+64]
	vrcpss xmm3, xmm2, [0x3b1]
