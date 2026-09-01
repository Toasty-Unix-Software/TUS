	vcomisd xmm3, xmm0
	vcomisd xmm1, xmm2
	vcomisd xmm5, xmm4
	vcomisd xmm5, xmm5

%ifdef ERROR
	vcomisd xmm10, xmm12
	vcomisd xmm11, xmm9
	vcomisd xmm26, xmm18
	vcomisd xmm25, xmm24
%endif
