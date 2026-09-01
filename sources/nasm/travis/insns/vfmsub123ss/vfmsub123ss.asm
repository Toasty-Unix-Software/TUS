default rel
	vfmsub123ss xmm3, xmm2, xmm4
	vfmsub123ss xmm2, xmm7, dword [0x3cc]
	vfmsub123ss xmm8, xmm12, xmm9
	vfmsub123ss xmm1, xmm4, dword [eax+1]
	vfmsub123ss xmm4, xmm7, dword [eax+64]
	vfmsub123ss xmm1, xmm4, [0xacc]
