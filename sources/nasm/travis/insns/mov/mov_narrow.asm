	mov al, [0x91f]
	mov al, [0xba]
	mov ax, [0xb8e]
	mov ax, [0xbae]
	mov eax, [0x4b2]
	mov eax, [0x28d]

%ifdef ERROR
	mov rax, [0x959]
	mov rax, [0x8ea]
	mov qword [eax+1], rbp
	mov qword [eax+64], rcx
	mov rdi, qword [eax+1]
	mov rbp, qword [eax+64]
	mov qword [eax+1], 141032669
	mov qword [eax+64], 141726563
	mov [0xf6e], rax
	mov rcx, [0x37d]
	mov [0x786], -349301823
%endif
