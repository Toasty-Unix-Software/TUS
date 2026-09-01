	pclmullqlqdq xmm6, oword [0xb6a]
	pclmullqlqdq xmm3, oword [0x694]

%ifdef ERROR
	pclmullqlqdq xmm8, xmm15
%endif
