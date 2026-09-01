default rel
	vfmsub213ss xmm0, xmm6, xmm0
	vfmsub213ss xmm5, xmm3, xmm2
	vfmsub213ss xmm4, xmm6, xmm1
	vfmsub213ss xmm7, xmm4, xmm7
	vfmsub213ss xmm12, xmm14, xmm14
	vfmsub213ss xmm10, xmm9, xmm9
	vfmsub213ss xmm23, xmm26, xmm19
	vfmsub213ss xmm30, xmm20, xmm22
	vfmsub213ss xmm7{k1}, xmm3, dword [0xde7]
	vfmsub213ss xmm7{k2}{z}, xmm3, xmm3
	vfmsub213ss xmm2, xmm7, xmm0, {rz-sae}
	vfmsub213ss xmm3, xmm5, dword [eax+1]
	vfmsub213ss xmm2, xmm3, dword [eax+64]
	vfmsub213ss xmm0, xmm4, dword [eax+1]
	vfmsub213ss xmm4, xmm4, dword [eax+64]
	vfmsub213ss xmm6, xmm4, [0x106]
	vfmsub213ss xmm4, xmm4, [0xa9e]
