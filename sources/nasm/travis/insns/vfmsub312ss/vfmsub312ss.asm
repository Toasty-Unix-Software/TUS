default rel
	vfmsub312ss xmm2, xmm6, dword [0x4db]
	vfmsub312ss xmm0, xmm1, xmm7
	vfmsub312ss xmm11, xmm8, xmm10
	vfmsub312ss xmm3, xmm4, dword [eax+1]
	vfmsub312ss xmm7, xmm4, dword [eax+64]
	vfmsub312ss xmm5, xmm6, [0x11a]
