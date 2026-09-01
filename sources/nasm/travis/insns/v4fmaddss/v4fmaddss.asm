default rel
	v4fmaddss xmm3, xmm6, [0xffc]
	v4fmaddss xmm2, xmm0, [0xc0a]
	v4fmaddss xmm8, xmm14, [0x4a7]
	v4fmaddss xmm17, xmm16, [0x3cc]
	v4fmaddss xmm2{k2}, xmm7, [0xe89]
	v4fmaddss xmm5{k4}{z}, xmm2, [0x20b]
	v4fmaddss xmm5, xmm0, [eax+1]
	v4fmaddss xmm7, xmm6, [eax+64]
