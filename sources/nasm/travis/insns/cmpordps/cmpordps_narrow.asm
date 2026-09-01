	cmpordps xmm5, xmm3
	cmpordps xmm4, xmm0

%ifdef ERROR
	cmpordps xmm12, xmm13
%endif
