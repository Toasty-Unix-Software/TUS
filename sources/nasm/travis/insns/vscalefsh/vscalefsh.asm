default rel
	vscalefsh xmm6, xmm3, xmm2
	vscalefsh xmm6, xmm2
	vscalefsh xmm6, xmm7, xmm4
	vscalefsh xmm11, xmm14, xmm9
	vscalefsh xmm20, xmm27, xmm27
	vscalefsh xmm2{k1}, xmm0, xmm0
	vscalefsh xmm1{k2}{z}, xmm3, xmm5
	vscalefsh xmm3, xmm0, xmm4, {rz-sae}
	vscalefsh xmm3, xmm5, word [eax+1]
	vscalefsh xmm6, xmm3, word [eax+64]
	vscalefsh xmm4, xmm0, [0xcc8]
