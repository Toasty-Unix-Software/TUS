	pcmpistri xmm7, oword [0x65e], 0x72
	pcmpistri xmm1, xmm4, 0x83

%ifdef ERROR
	pcmpistri xmm11, xmm11, 0xbc
%endif
