default rel
	movhpd qword [0xcc3], xmm2
	movhpd qword [0xeee], xmm4
	movhpd xmm0, qword [0xffb]
	movhpd xmm4, qword [0xbef]
	movhpd qword [0xe14], xmm8
	movhpd xmm8, qword [0x29c]
	movhpd qword [eax+1], xmm7
	movhpd qword [eax+64], xmm4
	movhpd xmm7, qword [eax+1]
	movhpd xmm2, qword [eax+64]
	movhpd [0x2a6], xmm3
	movhpd xmm5, [0x382]
