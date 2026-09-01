default rel
	vfnmadd123ss xmm3, xmm5, xmm4
	vfnmadd123ss xmm1, xmm0, xmm5
	vfnmadd123ss xmm11, xmm8, xmm10
	vfnmadd123ss xmm6, xmm7, dword [eax+1]
	vfnmadd123ss xmm0, xmm2, dword [eax+64]
	vfnmadd123ss xmm7, xmm3, [0xa17]
