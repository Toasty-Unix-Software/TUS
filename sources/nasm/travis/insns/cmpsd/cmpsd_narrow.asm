	cmpsd
	cmpsd
	cmpsd xmm3, xmm3, 0xbd
	cmpsd xmm6, qword [0x58f], 0xf3

%ifdef ERROR
	cmpsd xmm10, xmm9, 0x47
%endif
