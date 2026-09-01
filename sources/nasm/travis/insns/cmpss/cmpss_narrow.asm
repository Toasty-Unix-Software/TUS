	cmpss xmm3, xmm2, 0xaf
	cmpss xmm0, dword [0x153], 0x95

%ifdef ERROR
	cmpss xmm15, xmm10, 0x12
%endif
