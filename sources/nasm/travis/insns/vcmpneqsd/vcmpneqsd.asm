default rel
	vcmpneqsd xmm0, xmm7, qword [0x185]
	vcmpneqsd xmm0, qword [0x185]
	vcmpneqsd xmm3, xmm1, xmm0
	vcmpneqsd k6, xmm1, qword [0x54b]
	vcmpneqsd k1, xmm1, xmm0
	vcmpneqsd xmm8, xmm11, xmm15
	vcmpneqsd k2, xmm11, xmm12
	vcmpneqsd k6{k4}, xmm3, xmm7
	vcmpneqsd k3, xmm6, xmm0, {sae}
	vcmpneqsd xmm7, xmm1, qword [eax+1]
	vcmpneqsd xmm1, xmm7, qword [eax+64]
	vcmpneqsd k6, xmm2, qword [eax+1]
	vcmpneqsd k5, xmm0, qword [eax+64]
	vcmpneqsd xmm0, xmm4, [0xd44]
	vcmpneqsd k1, xmm4, [0x63a]
