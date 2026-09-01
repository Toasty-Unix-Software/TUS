default rel
	movhps xmm4, qword [0x22d]
	movhps xmm3, qword [0x3b6]
	movhps qword [0x547], xmm0
	movhps qword [0xaa8], xmm6
	movhps xmm12, qword [0x7f2]
	movhps qword [0x30e], xmm9
	movhps xmm2, qword [eax+1]
	movhps xmm3, qword [eax+64]
	movhps qword [eax+1], xmm0
	movhps qword [eax+64], xmm1
	movhps xmm5, [0xeff]
	movhps [0xbab], xmm5
