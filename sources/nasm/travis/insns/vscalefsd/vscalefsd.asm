default rel
	vscalefsd xmm5, xmm1, qword [0x268]
	vscalefsd xmm5, qword [0x268]
	vscalefsd xmm2, xmm7, xmm2
	vscalefsd xmm14, xmm12, xmm14
	vscalefsd xmm16, xmm24, xmm25
	vscalefsd xmm7{k1}, xmm3, xmm3
	vscalefsd xmm4{k1}{z}, xmm3, qword [0x582]
	vscalefsd xmm1, xmm6, xmm5, {rn-sae}
	vscalefsd xmm7, xmm2, qword [eax+1]
	vscalefsd xmm5, xmm2, qword [eax+64]
	vscalefsd xmm2, xmm6, [0x60a]
