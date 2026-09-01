default rel
	vfnmsub213ss xmm3, xmm6, xmm7
	vfnmsub213ss xmm5, xmm0, dword [0x10e]
	vfnmsub213ss xmm7, xmm7, xmm6
	vfnmsub213ss xmm5, xmm3, xmm4
	vfnmsub213ss xmm11, xmm8, xmm9
	vfnmsub213ss xmm14, xmm8, xmm14
	vfnmsub213ss xmm24, xmm16, xmm19
	vfnmsub213ss xmm31, xmm18, xmm22
	vfnmsub213ss xmm5{k6}, xmm7, xmm3
	vfnmsub213ss xmm1{k5}{z}, xmm3, xmm2
	vfnmsub213ss xmm4, xmm5, xmm0, {rn-sae}
	vfnmsub213ss xmm5, xmm4, dword [eax+1]
	vfnmsub213ss xmm7, xmm5, dword [eax+64]
	vfnmsub213ss xmm6, xmm6, dword [eax+1]
	vfnmsub213ss xmm4, xmm3, dword [eax+64]
	vfnmsub213ss xmm1, xmm7, [0x4e2]
	vfnmsub213ss xmm2, xmm5, [0x2bb]
