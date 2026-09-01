default rel
	vfnmsub312ss xmm5, xmm7, xmm4
	vfnmsub312ss xmm1, xmm2, xmm7
	vfnmsub312ss xmm9, xmm8, xmm14
	vfnmsub312ss xmm5, xmm5, dword [eax+1]
	vfnmsub312ss xmm4, xmm2, dword [eax+64]
	vfnmsub312ss xmm2, xmm1, [0x18c]
