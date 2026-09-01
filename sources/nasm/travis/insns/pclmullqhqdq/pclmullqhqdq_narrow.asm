	pclmullqhqdq xmm1, xmm5
	pclmullqhqdq xmm6, oword [0xe44]

%ifdef ERROR
	pclmullqhqdq xmm12, xmm14
%endif
