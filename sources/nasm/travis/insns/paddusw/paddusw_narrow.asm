	paddusw mm7, mm6
	paddusw mm2, qword [0x41f]
	paddusw xmm3, xmm0
	paddusw xmm3, oword [0x2cf]

%ifdef ERROR
	paddusw xmm15, xmm15
%endif
