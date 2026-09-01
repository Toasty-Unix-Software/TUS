	pinsrd xmm3, eax, 0xd1
	pinsrd xmm5, dword [0x8be], 0x8d

%ifdef ERROR
	pinsrd xmm10, r12d, 0x5e
%endif
