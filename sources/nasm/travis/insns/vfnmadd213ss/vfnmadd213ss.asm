default rel
	vfnmadd213ss xmm2, xmm7, xmm4
	vfnmadd213ss xmm6, xmm4, xmm4
	vfnmadd213ss xmm6, xmm3, dword [0x2ff]
	vfnmadd213ss xmm7, xmm3, dword [0x452]
	vfnmadd213ss xmm10, xmm12, xmm10
	vfnmadd213ss xmm14, xmm15, xmm9
	vfnmadd213ss xmm21, xmm22, xmm21
	vfnmadd213ss xmm23, xmm23, xmm26
	vfnmadd213ss xmm4{k4}, xmm7, xmm2
	vfnmadd213ss xmm2{k7}{z}, xmm2, dword [0x94a]
	vfnmadd213ss xmm6, xmm0, xmm1, {ru-sae}
	vfnmadd213ss xmm3, xmm5, dword [eax+1]
	vfnmadd213ss xmm7, xmm5, dword [eax+64]
	vfnmadd213ss xmm3, xmm4, dword [eax+1]
	vfnmadd213ss xmm4, xmm2, dword [eax+64]
	vfnmadd213ss xmm3, xmm3, [0x97b]
	vfnmadd213ss xmm1, xmm3, [0x348]
