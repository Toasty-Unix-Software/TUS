default rel
	vfmsub231sh xmm4, xmm0, word [0xd58]
	vfmsub231sh xmm4, word [0xd58]
	vfmsub231sh xmm5, xmm6, xmm0
	vfmsub231sh xmm9, xmm10, xmm9
	vfmsub231sh xmm28, xmm30, xmm28
	vfmsub231sh xmm5{k4}, xmm1, word [0x902]
	vfmsub231sh xmm5{k3}{z}, xmm2, xmm6
	vfmsub231sh xmm0, xmm2, xmm2, {ru-sae}
	vfmsub231sh xmm4, xmm4, word [eax+1]
	vfmsub231sh xmm1, xmm0, word [eax+64]
	vfmsub231sh xmm7, xmm4, [0xeb7]
