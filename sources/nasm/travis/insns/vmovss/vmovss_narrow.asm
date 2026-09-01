	vmovss xmm3, xmm4, xmm7
	vmovss xmm3, xmm7
	vmovss xmm2, xmm1, xmm1
	vmovss xmm4, dword [0xb30]
	vmovss xmm6, dword [0xdc6]
	vmovss dword [0x66f], xmm2
	vmovss dword [0x809], xmm3
	vmovss xmm6, dword [0xc04]
	vmovss xmm0, dword [0x7e9]

%ifdef ERROR
	vmovss xmm15, xmm10, xmm12
	vmovss xmm8, dword [0x214]
	vmovss dword [0x94a], xmm9
	vmovss xmm9, dword [0xa3a]
	vmovss xmm31, xmm31, xmm25
	vmovss xmm28, dword [0xf7c]
	vmovss dword [0xc59], xmm16
	vmovss xmm18, dword [0x22e]
%endif
