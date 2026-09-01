	paddw mm4, mm5
	paddw mm2, qword [0x306]
	paddw xmm0, xmm4
	paddw xmm4, oword [0x28a]

%ifdef ERROR
	paddw xmm14, xmm9
%endif
