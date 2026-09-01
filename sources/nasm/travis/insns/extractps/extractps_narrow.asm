	extractps dword [0x687], xmm5, 0x74
	extractps edi, xmm0, 0xb3

%ifdef ERROR
	extractps rcx, xmm7, 0x5e
	extractps rbx, xmm2, 0x45
	extractps r15d, xmm11, 0x8b
	extractps r14, xmm9, 0x5c
%endif
