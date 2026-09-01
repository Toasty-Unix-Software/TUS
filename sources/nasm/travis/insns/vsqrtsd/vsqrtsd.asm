default rel
	vsqrtsd xmm2, xmm6, xmm0
	vsqrtsd xmm2, xmm0
	vsqrtsd xmm7, xmm3, xmm3
	vsqrtsd xmm7, xmm7, xmm7
	vsqrtsd xmm7, xmm7
	vsqrtsd xmm2, xmm4, xmm6
	vsqrtsd xmm15, xmm9, xmm8
	vsqrtsd xmm12, xmm13, xmm10
	vsqrtsd xmm20, xmm28, xmm19
	vsqrtsd xmm22, xmm26, xmm27
	vsqrtsd xmm1{k3}, xmm5, xmm5
	vsqrtsd xmm4{k2}{z}, xmm5, xmm1
	vsqrtsd xmm5, xmm5, xmm2, {ru-sae}
	vsqrtsd xmm4, xmm6, qword [eax+1]
	vsqrtsd xmm7, xmm5, qword [eax+64]
	vsqrtsd xmm3, xmm2, qword [eax+1]
	vsqrtsd xmm0, xmm2, qword [eax+64]
	vsqrtsd xmm7, xmm6, [0xba0]
	vsqrtsd xmm1, xmm3, [0x210]
