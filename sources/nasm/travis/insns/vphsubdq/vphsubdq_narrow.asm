	vphsubdq xmm2, xmm7
	vphsubdq xmm2
	vphsubdq xmm5, xmm2

%ifdef ERROR
	vphsubdq xmm12, xmm11
%endif
