	pmovzxbw xmm1, xmm4
	pmovzxbw xmm7, qword [0xd61]

%ifdef ERROR
	pmovzxbw xmm11, xmm12
%endif
