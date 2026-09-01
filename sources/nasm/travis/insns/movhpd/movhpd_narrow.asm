	movhpd qword [0xcc3], xmm2
	movhpd qword [0xeee], xmm4
	movhpd xmm0, qword [0xffb]
	movhpd xmm4, qword [0xbef]

%ifdef ERROR
	movhpd qword [0xe14], xmm8
	movhpd xmm8, qword [0x29c]
%endif
