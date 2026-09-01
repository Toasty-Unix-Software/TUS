default rel
	rcpss xmm7, xmm7
	rcpss xmm4, dword [0xd1d]
	rcpss xmm8, xmm12
	rcpss xmm4, dword [eax+1]
	rcpss xmm7, dword [eax+64]
	rcpss xmm0, [0x235]
