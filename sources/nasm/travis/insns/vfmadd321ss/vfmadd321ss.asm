default rel
	vfmadd321ss xmm6, xmm2, dword [0x3db]
	vfmadd321ss xmm7, xmm6, xmm7
	vfmadd321ss xmm14, xmm13, xmm10
	vfmadd321ss xmm5, xmm1, dword [eax+1]
	vfmadd321ss xmm1, xmm6, dword [eax+64]
	vfmadd321ss xmm3, xmm1, [0xb38]
