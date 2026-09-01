default rel
	cvtdq2pd xmm6, oword [0x308]
	cvtdq2pd xmm5, oword [0xe40]
	cvtdq2pd xmm10, xmm9
	cvtdq2pd xmm5, oword [eax+1]
	cvtdq2pd xmm7, oword [eax+64]
	cvtdq2pd xmm2, [0xe9a]
