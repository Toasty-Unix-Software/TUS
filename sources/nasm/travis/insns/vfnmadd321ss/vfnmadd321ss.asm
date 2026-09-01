default rel
	vfnmadd321ss xmm0, xmm7, xmm1
	vfnmadd321ss xmm2, xmm3, dword [0x4c4]
	vfnmadd321ss xmm9, xmm14, xmm8
	vfnmadd321ss xmm0, xmm1, dword [eax+1]
	vfnmadd321ss xmm7, xmm0, dword [eax+64]
	vfnmadd321ss xmm2, xmm7, [0xad7]
