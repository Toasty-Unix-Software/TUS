	psubb mm1, qword [0x5b7]
	psubb mm1, qword [0xc2a]
	psubb xmm0, xmm6
	psubb xmm5, oword [0xbe0]

%ifdef ERROR
	psubb xmm15, xmm8
%endif
