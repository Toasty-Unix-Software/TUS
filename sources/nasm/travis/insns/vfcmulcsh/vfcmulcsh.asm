default rel
	vfcmulcsh xmm2, xmm2, dword [0x34a]
	vfcmulcsh xmm2, dword [0x34a]
	vfcmulcsh xmm1, xmm0, xmm6
	vfcmulcsh xmm12, xmm10, xmm12
	vfcmulcsh xmm21, xmm23, xmm20
	vfcmulcsh xmm6{k5}, xmm1, dword [0x209]
	vfcmulcsh xmm3{k3}{z}, xmm7, dword [0xb4f]
	vfcmulcsh xmm7, xmm6, xmm1, {ru-sae}
	vfcmulcsh xmm6, xmm4, dword [eax+1]
	vfcmulcsh xmm7, xmm4, dword [eax+64]
	vfcmulcsh xmm5, xmm6, [0x7b5]
