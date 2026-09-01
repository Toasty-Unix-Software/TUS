default rel
	vgetexpss xmm4, xmm3, dword [0xed3]
	vgetexpss xmm3, xmm0, xmm5
	vgetexpss xmm11, xmm15, xmm13
	vgetexpss xmm21, xmm21, xmm16
	vgetexpss xmm5{k1}, xmm2, xmm7
	vgetexpss xmm0{k4}{z}, xmm2, dword [0xc0d]
	vgetexpss xmm0, xmm7, xmm4, {sae}
	vgetexpss xmm7, xmm4, dword [eax+1]
	vgetexpss xmm7, xmm3, dword [eax+64]
	vgetexpss xmm2, xmm6, [0x1f0]
