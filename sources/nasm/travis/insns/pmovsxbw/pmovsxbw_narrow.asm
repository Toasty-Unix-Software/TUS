	pmovsxbw xmm2, qword [0x693]
	pmovsxbw xmm5, xmm0

%ifdef ERROR
	pmovsxbw xmm12, xmm11
%endif
