default rel
	vfcmaddcsh xmm2, xmm3, dword [0x392]
	vfcmaddcsh xmm2, dword [0x392]
	vfcmaddcsh xmm0, xmm6, xmm0
	vfcmaddcsh xmm10, xmm9, xmm11
	vfcmaddcsh xmm16, xmm21, xmm20
	vfcmaddcsh xmm7{k4}, xmm7, xmm1
	vfcmaddcsh xmm3{k2}{z}, xmm1, xmm2
	vfcmaddcsh xmm6, xmm0, xmm5, {ru-sae}
	vfcmaddcsh xmm6, xmm2, dword [eax+1]
	vfcmaddcsh xmm0, xmm3, dword [eax+64]
	vfcmaddcsh xmm3, xmm4, [0xa49]
