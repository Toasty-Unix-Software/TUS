	movhps xmm4, qword [0x22d]
	movhps xmm3, qword [0x3b6]
	movhps qword [0x547], xmm0
	movhps qword [0xaa8], xmm6

%ifdef ERROR
	movhps xmm12, qword [0x7f2]
	movhps qword [0x30e], xmm9
%endif
