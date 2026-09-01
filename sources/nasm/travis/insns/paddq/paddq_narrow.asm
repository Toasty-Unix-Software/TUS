	paddq mm7, mm2
	paddq mm1, qword [0x86a]
	paddq xmm7, xmm1
	paddq xmm2, xmm3

%ifdef ERROR
	paddq xmm13, xmm10
%endif
