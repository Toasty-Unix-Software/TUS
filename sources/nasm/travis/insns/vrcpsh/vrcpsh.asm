default rel
	vrcpsh xmm6, xmm6, xmm3
	vrcpsh xmm6, xmm3
	vrcpsh xmm5, xmm3, xmm5
	vrcpsh xmm12, xmm12, xmm10
	vrcpsh xmm26, xmm27, xmm22
	vrcpsh xmm2{k7}, xmm2, xmm3
	vrcpsh xmm0{k6}{z}, xmm7, xmm3
	vrcpsh xmm4, xmm5, xmm4, {sae}
	vrcpsh xmm3, xmm1, word [eax+1]
	vrcpsh xmm5, xmm0, word [eax+64]
	vrcpsh xmm2, xmm1, [0x2a9]
