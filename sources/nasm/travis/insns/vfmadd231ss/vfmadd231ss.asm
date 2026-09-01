default rel
	vfmadd231ss xmm0, xmm1, dword [0x388]
	vfmadd231ss xmm7, xmm4, xmm1
	vfmadd231ss xmm4, xmm6, dword [0x582]
	vfmadd231ss xmm7, xmm6, xmm3
	vfmadd231ss xmm14, xmm12, xmm11
	vfmadd231ss xmm13, xmm14, xmm8
	vfmadd231ss xmm17, xmm18, xmm16
	vfmadd231ss xmm24, xmm26, xmm25
	vfmadd231ss xmm2{k1}, xmm3, xmm0
	vfmadd231ss xmm6{k1}{z}, xmm0, dword [0x2a0]
	vfmadd231ss xmm2, xmm4, xmm5, {rn-sae}
	vfmadd231ss xmm0, xmm3, dword [eax+1]
	vfmadd231ss xmm3, xmm6, dword [eax+64]
	vfmadd231ss xmm7, xmm5, dword [eax+1]
	vfmadd231ss xmm1, xmm2, dword [eax+64]
	vfmadd231ss xmm3, xmm7, [0xd07]
	vfmadd231ss xmm4, xmm7, [0xbf9]
