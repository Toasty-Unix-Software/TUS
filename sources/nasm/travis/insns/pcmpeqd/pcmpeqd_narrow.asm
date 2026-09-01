	pcmpeqd mm4, qword [0x5d6]
	pcmpeqd mm6, qword [0xdae]
	pcmpeqd xmm0, oword [0x8eb]
	pcmpeqd xmm5, oword [0xee9]

%ifdef ERROR
	pcmpeqd xmm15, xmm9
%endif
