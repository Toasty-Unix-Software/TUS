	pminud xmm1, oword [0xf94]
	pminud xmm0, xmm0

%ifdef ERROR
	pminud xmm12, xmm10
%endif
