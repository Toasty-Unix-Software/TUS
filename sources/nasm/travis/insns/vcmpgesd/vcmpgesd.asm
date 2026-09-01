default rel
	vcmpgesd xmm3, xmm4, xmm3
	vcmpgesd xmm3, xmm3
	vcmpgesd xmm5, xmm6, xmm6
	vcmpgesd k7, xmm7, xmm4
	vcmpgesd k4, xmm4, qword [0x7b8]
	vcmpgesd xmm8, xmm10, xmm12
	vcmpgesd k1, xmm11, xmm10
	vcmpgesd k2{k4}, xmm1, xmm6
	vcmpgesd k2, xmm7, xmm6, {sae}
	vcmpgesd xmm6, xmm0, qword [eax+1]
	vcmpgesd xmm0, xmm6, qword [eax+64]
	vcmpgesd k1, xmm2, qword [eax+1]
	vcmpgesd k6, xmm1, qword [eax+64]
	vcmpgesd xmm0, xmm6, [0x7ba]
	vcmpgesd k2, xmm6, [0x438]
