	cmpordss xmm6, xmm7
	cmpordss xmm2, dword [0x392]

%ifdef ERROR
	cmpordss xmm15, xmm10
%endif
