	roundss xmm3, xmm4, 0x2c
	roundss xmm3, dword [0xd4a], 0xa1

%ifdef ERROR
	roundss xmm10, xmm13, 0x8
%endif
