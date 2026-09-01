default rel
	cvtpi2pd xmm1, mm4
	cvtpi2pd xmm7, mm3
	cvtpi2pd xmm12, mm1
	cvtpi2pd xmm0, qword [eax+1]
	cvtpi2pd xmm1, qword [eax+64]
	cvtpi2pd xmm5, [0x525]
