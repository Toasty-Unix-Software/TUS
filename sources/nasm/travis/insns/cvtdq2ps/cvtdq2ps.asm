default rel
	cvtdq2ps xmm6, xmm6
	cvtdq2ps xmm6, xmm5
	cvtdq2ps xmm15, xmm10
	cvtdq2ps xmm0, oword [eax+1]
	cvtdq2ps xmm5, oword [eax+64]
	cvtdq2ps xmm0, [0x63f]
