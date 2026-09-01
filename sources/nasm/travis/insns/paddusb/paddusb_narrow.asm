	paddusb mm4, qword [0xb3f]
	paddusb mm0, mm5
	paddusb xmm1, xmm1
	paddusb xmm5, oword [0xcfe]

%ifdef ERROR
	paddusb xmm9, xmm11
%endif
