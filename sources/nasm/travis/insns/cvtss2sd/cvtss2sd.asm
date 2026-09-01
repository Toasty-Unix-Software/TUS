default rel
	cvtss2sd xmm5, xmm1
	cvtss2sd xmm6, xmm7
	cvtss2sd xmm14, xmm10
	cvtss2sd xmm3, oword [eax+1]
	cvtss2sd xmm5, oword [eax+64]
	cvtss2sd xmm0, [0x21d]
