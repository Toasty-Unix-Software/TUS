default rel
	vminsd xmm2, xmm7, qword [0xb83]
	vminsd xmm2, qword [0xb83]
	vminsd xmm4, xmm4, xmm4
	vminsd xmm3, xmm2, xmm0
	vminsd xmm3, xmm0
	vminsd xmm2, xmm3, xmm7
	vminsd xmm9, xmm14, xmm10
	vminsd xmm13, xmm12, xmm9
	vminsd xmm18, xmm31, xmm27
	vminsd xmm30, xmm16, xmm26
	vminsd xmm5{k1}, xmm3, xmm4
	vminsd xmm5{k7}{z}, xmm5, xmm7
	vminsd xmm4, xmm2, xmm2, {sae}
	vminsd xmm4, xmm2, qword [eax+1]
	vminsd xmm5, xmm3, qword [eax+64]
	vminsd xmm0, xmm5, qword [eax+1]
	vminsd xmm4, xmm0, qword [eax+64]
	vminsd xmm0, xmm1, [0x702]
	vminsd xmm2, xmm5, [0xd63]
