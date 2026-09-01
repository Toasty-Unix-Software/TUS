default rel
	cvtps2pi mm6, qword [0x4b6]
	cvtps2pi mm1, qword [0x483]
	cvtps2pi mm6, xmm15
	cvtps2pi mm3, qword [eax+1]
	cvtps2pi mm6, qword [eax+64]
	cvtps2pi mm3, [0xc81]
