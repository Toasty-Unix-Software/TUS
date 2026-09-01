default rel
	vfmadd312ss xmm0, xmm7, dword [0x62c]
	vfmadd312ss xmm7, xmm7, xmm7
	vfmadd312ss xmm11, xmm8, xmm14
	vfmadd312ss xmm7, xmm0, dword [eax+1]
	vfmadd312ss xmm4, xmm4, dword [eax+64]
	vfmadd312ss xmm6, xmm0, [0xe01]
