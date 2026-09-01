default rel
	vfnmadd312ss xmm7, xmm4, xmm3
	vfnmadd312ss xmm5, xmm0, xmm7
	vfnmadd312ss xmm15, xmm11, xmm14
	vfnmadd312ss xmm4, xmm3, dword [eax+1]
	vfnmadd312ss xmm2, xmm0, dword [eax+64]
	vfnmadd312ss xmm0, xmm6, [0xcca]
