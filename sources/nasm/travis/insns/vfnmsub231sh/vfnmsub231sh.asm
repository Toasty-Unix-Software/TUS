default rel
	vfnmsub231sh xmm7, xmm6, word [0xc0e]
	vfnmsub231sh xmm7, word [0xc0e]
	vfnmsub231sh xmm3, xmm0, word [0x9d6]
	vfnmsub231sh xmm12, xmm9, xmm12
	vfnmsub231sh xmm22, xmm28, xmm20
	vfnmsub231sh xmm0{k6}, xmm7, word [0xc59]
	vfnmsub231sh xmm4{k1}{z}, xmm1, word [0xc41]
	vfnmsub231sh xmm0, xmm2, xmm1, {rn-sae}
	vfnmsub231sh xmm3, xmm1, word [eax+1]
	vfnmsub231sh xmm6, xmm5, word [eax+64]
	vfnmsub231sh xmm5, xmm3, [0x590]
