default rel
	vcmpngtsd xmm5, xmm4, xmm4
	vcmpngtsd xmm5, xmm4
	vcmpngtsd xmm3, xmm1, qword [0x431]
	vcmpngtsd k5, xmm2, xmm0
	vcmpngtsd k6, xmm7, xmm7
	vcmpngtsd xmm13, xmm8, xmm12
	vcmpngtsd k5, xmm12, xmm13
	vcmpngtsd k5{k4}, xmm3, xmm1
	vcmpngtsd k1, xmm6, xmm3, {sae}
	vcmpngtsd xmm1, xmm6, qword [eax+1]
	vcmpngtsd xmm6, xmm2, qword [eax+64]
	vcmpngtsd k6, xmm2, qword [eax+1]
	vcmpngtsd k6, xmm5, qword [eax+64]
	vcmpngtsd xmm3, xmm6, [0xc44]
	vcmpngtsd k3, xmm4, [0x1ff]
