	cmpeqsd xmm2, oword [0xe2e]
	cmpeqsd xmm2, xmm3

%ifdef ERROR
	cmpeqsd xmm11, xmm13
%endif
