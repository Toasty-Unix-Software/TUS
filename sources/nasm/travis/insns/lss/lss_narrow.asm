	lss di, word [0xcbe]
	lss cx, word [0xca7]
	lss edi, dword [0x31a]
	lss edx, dword [0x3ac]

%ifdef ERROR
	lss rdi, qword [0x29e]
	lss rax, qword [0xa43]
	lss r15w, word [0xed2]
	lss r12d, dword [0x4c0]
	lss r15, qword [0xfc2]
	lss r29w, word [0x227]
	lss r19d, dword [0x1f0]
	lss r19, qword [0x6b9]
	lss rsi, qword [eax+1]
	lss rbx, qword [eax+64]
	lss rsi, [0x9da]
%endif
