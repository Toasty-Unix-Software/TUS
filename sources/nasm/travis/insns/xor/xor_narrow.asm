	xor bl, dl
	xor byte [0x189], dl
	xor ax, dx
	xor bx, ax
	xor dword [0x854], edi
	xor ebp, ecx

%ifdef ERROR
	xor rdi, rdx
	xor rbp, rdx
	xor r11b, r11b
	xor r8w, r14w
	xor r13d, r12d
	xor r14, r12
	xor r22b, r16b
	xor r27w, r18w
	xor r16d, r31d
	xor r18, r20
	xor qword [eax+1], rdi
	xor qword [eax+64], rax
	xor rbp, qword [eax+1]
	xor rbp, qword [eax+64]
	xor qword [eax+1], -106
	xor qword [eax+64], -18
	xor qword [eax+1], -461772592
	xor qword [eax+64], -177753161
	xor rdx, rax, qword [eax+1]
	xor rdi, rcx, qword [eax+64]
	xor rdi, qword [eax+1], rcx
	xor rbx, qword [eax+64], rax
	xor rdx, qword [eax+1], -114
	xor rbp, qword [eax+64], -114
	xor rcx, qword [eax+1], -167674078
	xor rax, qword [eax+64], 206588672
	xor [0xab4], rcx
	xor rbx, [0x6fd]
	xor [0xa1c], -77
	xor [0x838], 201469660
	xor rsi, rdx, [0xc8d]
	xor rdi, [0x9bc], rsi
	xor rdi, [0x1ae], -88
	xor rbx, [0x945], 191341903
%endif
