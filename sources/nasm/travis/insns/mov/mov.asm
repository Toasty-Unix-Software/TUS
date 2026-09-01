default rel
	mov al, [0x91f]
	mov al, [0xba]
	mov ax, [0xb8e]
	mov ax, [0xbae]
	mov eax, [0x4b2]
	mov eax, [0x28d]
	mov rax, [0x959]
	mov rax, [0x8ea]
	mov byte [eax+1], bl
	mov byte [eax+64], al
	mov word [eax+1], dx
	mov word [eax+64], bx
	mov dword [eax+1], esi
	mov dword [eax+64], ebp
	mov qword [eax+1], rbp
	mov qword [eax+64], rcx
	mov cl, byte [eax+1]
	mov cl, byte [eax+64]
	mov dx, word [eax+1]
	mov dx, word [eax+64]
	mov ecx, dword [eax+1]
	mov edi, dword [eax+64]
	mov rdi, qword [eax+1]
	mov rbp, qword [eax+64]
	mov byte [eax+1], 0x5f
	mov byte [eax+64], 0x18
	mov word [eax+1], 0x8884
	mov word [eax+64], 0x1f41
	mov dword [eax+1], 0xa787858
	mov dword [eax+64], 0x2b22df3e
	mov qword [eax+1], 141032669
	mov qword [eax+64], 141726563
	mov word [eax+1], gs
	mov word [eax+64], gs
	mov gs, word [eax+1]
	mov gs, word [eax+64]
	mov byte [eax+1], k4
	mov byte [eax+64], k6
	mov word [eax+1], k1
	mov word [eax+64], k3
	mov dword [eax+1], k6
	mov dword [eax+64], k1
	mov qword [eax+1], k2
	mov qword [eax+64], k6
	mov [0xd7d], al
	mov [0x1eb], si
	mov [0x298], edx
	mov [0xf6e], rax
	mov dl, [0xddf]
	mov bx, [0xa73]
	mov edi, [0x327]
	mov rcx, [0x37d]
	mov [0x514], 0xa0
	mov [0xdbb], 0xedea
	mov [0x18e], 0x3e76489d
	mov [0x786], -349301823
	mov [0x501], es
	mov fs, [0xeb4]
	mov [0x348], k5
	mov [0xa2b], k7
	mov [0x25c], k6
	mov [0x4f8], k7
