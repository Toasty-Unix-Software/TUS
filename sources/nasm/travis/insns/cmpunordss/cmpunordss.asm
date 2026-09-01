default rel
	cmpunordss xmm1, xmm7
	cmpunordss xmm4, xmm6
	cmpunordss xmm8, xmm9
	cmpunordss xmm3, dword [eax+1]
	cmpunordss xmm0, dword [eax+64]
	cmpunordss xmm6, [0x8d7]
