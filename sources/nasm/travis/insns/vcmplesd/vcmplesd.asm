default rel
	vcmplesd xmm6, xmm7, qword [0xb0b]
	vcmplesd xmm6, qword [0xb0b]
	vcmplesd xmm0, xmm1, xmm4
	vcmplesd k3, xmm4, xmm6
	vcmplesd k1, xmm2, xmm2
	vcmplesd xmm13, xmm12, xmm9
	vcmplesd k1, xmm15, xmm8
	vcmplesd k5{k3}, xmm4, xmm4
	vcmplesd k6, xmm6, xmm0, {sae}
	vcmplesd xmm6, xmm6, qword [eax+1]
	vcmplesd xmm3, xmm5, qword [eax+64]
	vcmplesd k1, xmm4, qword [eax+1]
	vcmplesd k7, xmm1, qword [eax+64]
	vcmplesd xmm0, xmm0, [0x6d6]
	vcmplesd k6, xmm3, [0xf94]
