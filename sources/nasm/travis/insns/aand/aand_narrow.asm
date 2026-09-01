	aand dword [0x9d7], edi
	aand dword [0xa64], ebx

%ifdef ERROR
	aand qword [0x371], rbp
	aand qword [0xf0b], rbx
	aand dword [0x32c], r14d
	aand qword [0x765], r9
	aand dword [0x205], r28d
	aand qword [0x3bd], r23
	aand qword [eax+1], rdx
	aand qword [eax+64], rsi
	aand [0x77b], rdx
%endif
