	pabsb mm6, qword [0x4a2]
	pabsb mm6, qword [0x402]
	pabsb xmm0, xmm6
	pabsb xmm2, oword [0x4fb]

%ifdef ERROR
	pabsb xmm15, xmm15
%endif
