default rel
	vfmadd123ss xmm0, xmm5, xmm0
	vfmadd123ss xmm7, xmm2, xmm5
	vfmadd123ss xmm9, xmm9, xmm9
	vfmadd123ss xmm0, xmm4, dword [eax+1]
	vfmadd123ss xmm1, xmm3, dword [eax+64]
	vfmadd123ss xmm5, xmm2, [0xf18]
