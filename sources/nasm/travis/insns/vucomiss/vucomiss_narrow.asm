	vucomiss xmm3, dword [0xb67]
	vucomiss xmm4, dword [0x92a]
	vucomiss xmm4, xmm2
	vucomiss xmm2, dword [0x20b]

%ifdef ERROR
	vucomiss xmm10, xmm8
	vucomiss xmm12, xmm13
	vucomiss xmm17, xmm25
	vucomiss xmm28, xmm24
%endif
