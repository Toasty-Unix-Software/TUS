	vreducess xmm4, xmm4, xmm1, 0xfc
	vreducess xmm4, xmm1, 0xfc
	vreducess xmm7, xmm0, dword [0xeca], 0xcd

%ifdef ERROR
	vreducess xmm11, xmm11, xmm12, 0x2b
	vreducess xmm24, xmm17, xmm20, 0x68
%endif
