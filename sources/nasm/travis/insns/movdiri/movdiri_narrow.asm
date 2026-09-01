	movdiri dword [0xf28], edx
	movdiri dword [0x345], ebx

%ifdef ERROR
	movdiri qword [0x770], rcx
	movdiri qword [0x9f0], rax
	movdiri dword [0x16d], r14d
	movdiri qword [0x3dd], r12
	movdiri dword [0x16a], r29d
	movdiri qword [0xa38], r29
	movdiri qword [eax+1], rbp
	movdiri qword [eax+64], rsi
	movdiri [0x184], rbx
%endif
