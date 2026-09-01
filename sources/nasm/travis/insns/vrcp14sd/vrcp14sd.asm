default rel
	vrcp14sd xmm3, xmm3, qword [0x9f2]
	vrcp14sd xmm3, qword [0x9f2]
	vrcp14sd xmm2, xmm3, qword [0x1e0]
	vrcp14sd xmm10, xmm8, xmm15
	vrcp14sd xmm22, xmm23, xmm20
	vrcp14sd xmm5{k6}, xmm2, xmm1
	vrcp14sd xmm0{k1}{z}, xmm4, qword [0x9de]
	vrcp14sd xmm0, xmm4, qword [eax+1]
	vrcp14sd xmm3, xmm7, qword [eax+64]
	vrcp14sd xmm5, xmm0, [0xc74]
