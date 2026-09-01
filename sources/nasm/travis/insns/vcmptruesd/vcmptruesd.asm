default rel
	vcmptruesd xmm1, xmm4, xmm7
	vcmptruesd xmm1, xmm7
	vcmptruesd xmm2, xmm3, qword [0xa5b]
	vcmptruesd k7, xmm7, xmm0
	vcmptruesd k4, xmm5, xmm4
	vcmptruesd xmm12, xmm11, xmm12
	vcmptruesd k3, xmm12, xmm10
	vcmptruesd k6{k6}, xmm1, xmm6
	vcmptruesd k2, xmm7, xmm1, {sae}
	vcmptruesd xmm0, xmm2, qword [eax+1]
	vcmptruesd xmm2, xmm2, qword [eax+64]
	vcmptruesd k1, xmm7, qword [eax+1]
	vcmptruesd k4, xmm6, qword [eax+64]
	vcmptruesd xmm3, xmm5, [0x4dc]
	vcmptruesd k7, xmm1, [0x383]
