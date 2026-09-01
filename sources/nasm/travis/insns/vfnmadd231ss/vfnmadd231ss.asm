default rel
	vfnmadd231ss xmm0, xmm5, xmm1
	vfnmadd231ss xmm0, xmm5, xmm1
	vfnmadd231ss xmm3, xmm6, xmm0
	vfnmadd231ss xmm2, xmm2, xmm2
	vfnmadd231ss xmm13, xmm9, xmm9
	vfnmadd231ss xmm14, xmm9, xmm14
	vfnmadd231ss xmm16, xmm22, xmm18
	vfnmadd231ss xmm16, xmm17, xmm19
	vfnmadd231ss xmm2{k4}, xmm6, dword [0xcda]
	vfnmadd231ss xmm3{k3}{z}, xmm5, xmm2
	vfnmadd231ss xmm0, xmm7, xmm7, {rd-sae}
	vfnmadd231ss xmm6, xmm7, dword [eax+1]
	vfnmadd231ss xmm3, xmm2, dword [eax+64]
	vfnmadd231ss xmm7, xmm2, dword [eax+1]
	vfnmadd231ss xmm3, xmm2, dword [eax+64]
	vfnmadd231ss xmm3, xmm1, [0xa88]
	vfnmadd231ss xmm4, xmm2, [0xf21]
