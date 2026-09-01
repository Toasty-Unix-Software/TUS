default rel
	cvtsd2ss xmm5, xmm3
	cvtsd2ss xmm2, xmm1
	cvtsd2ss xmm13, xmm10
	cvtsd2ss xmm5, qword [eax+1]
	cvtsd2ss xmm0, qword [eax+64]
	cvtsd2ss xmm5, [0x6fc]
