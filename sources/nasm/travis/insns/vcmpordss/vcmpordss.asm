default rel
	vcmpordss xmm3, xmm3, xmm5
	vcmpordss xmm3, xmm5
	vcmpordss xmm4, xmm7, xmm7
	vcmpordss k5, xmm0, dword [0x57a]
	vcmpordss k2, xmm4, xmm0
	vcmpordss xmm12, xmm13, xmm9
	vcmpordss k3, xmm14, xmm15
	vcmpordss k2{k3}, xmm4, xmm0
	vcmpordss k3, xmm3, xmm0, {sae}
	vcmpordss xmm6, xmm6, qword [eax+1]
	vcmpordss xmm7, xmm0, qword [eax+64]
	vcmpordss k4, xmm4, dword [eax+1]
	vcmpordss k5, xmm1, dword [eax+64]
	vcmpordss xmm3, xmm6, [0xc6e]
	vcmpordss k3, xmm4, [0x1a8]
