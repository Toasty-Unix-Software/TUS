default rel
	vgetexpsd xmm0, xmm7, xmm0
	vgetexpsd xmm1, xmm6, qword [0x121]
	vgetexpsd xmm12, xmm12, xmm10
	vgetexpsd xmm26, xmm25, xmm19
	vgetexpsd xmm5{k2}, xmm6, xmm6
	vgetexpsd xmm0{k3}{z}, xmm0, xmm3
	vgetexpsd xmm3, xmm6, xmm1, {sae}
	vgetexpsd xmm5, xmm2, qword [eax+1]
	vgetexpsd xmm0, xmm1, qword [eax+64]
	vgetexpsd xmm4, xmm7, [0x5cf]
