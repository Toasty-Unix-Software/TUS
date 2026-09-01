	axor dword [0xa02], esi
	axor dword [0xe3e], esi

%ifdef ERROR
	axor qword [0x313], rdx
	axor qword [0x652], rcx
	axor dword [0x49b], r12d
	axor qword [0xeea], r10
	axor dword [0x301], r30d
	axor qword [0xee2], r24
	axor qword [eax+1], rbp
	axor qword [eax+64], rsi
	axor [0xa5c], rax
%endif
