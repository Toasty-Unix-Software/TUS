	vcmpordss xmm3, xmm3, xmm5
	vcmpordss xmm3, xmm5
	vcmpordss xmm4, xmm7, xmm7
	vcmpordss k5, xmm0, dword [0x57a]
	vcmpordss k2, xmm4, xmm0

%ifdef ERROR
	vcmpordss xmm12, xmm13, xmm9
	vcmpordss k3, xmm14, xmm15
%endif
