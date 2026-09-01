default rel
	ucomisd xmm3, xmm5
	ucomisd xmm2, xmm7
	ucomisd xmm11, xmm8
	ucomisd xmm3, qword [eax+1]
	ucomisd xmm7, qword [eax+64]
	ucomisd xmm5, [0x79d]
