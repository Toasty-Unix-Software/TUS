default rel
	vgetmantsd xmm1, xmm3, xmm6, 0x8d
	vgetmantsd xmm4, xmm4, xmm0, 0x29
	vgetmantsd xmm8, xmm10, xmm8, 0x3f
	vgetmantsd xmm30, xmm27, xmm27, 0x76
	vgetmantsd xmm1{k2}, xmm5, xmm6, 0x76
	vgetmantsd xmm3{k2}{z}, xmm3, xmm3, 0x98
	vgetmantsd xmm3, xmm6, qword [eax+1], 0xef
	vgetmantsd xmm1, xmm3, qword [eax+64], 0xaf
	vgetmantsd xmm6, xmm6, [0x95d], 0x9f
