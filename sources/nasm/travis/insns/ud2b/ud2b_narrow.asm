	ud2b si, di
	ud2b bx, word [0x12f]
	ud2b ecx, ecx
	ud2b edi, ebp
	ud2b
	ud2b

%ifdef ERROR
	ud2b rbp, rdx
	ud2b rbx, rax
	ud2b r14w, r13w
	ud2b r8d, r9d
	ud2b r9, r11
	ud2b r18w, r24w
	ud2b r18d, r16d
	ud2b r17, r21
	ud2b rbx, qword [eax+1]
	ud2b rdi, qword [eax+64]
	ud2b rcx, [0xbe6]
%endif
