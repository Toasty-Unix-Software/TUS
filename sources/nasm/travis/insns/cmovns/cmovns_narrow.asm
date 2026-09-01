	cmovns dx, ax
	cmovns cx, word [0x70e]
	cmovns ebx, edi
	cmovns eax, eax
	cmovns ax, di, si
	cmovns bx, bp, word [0x989]

%ifdef ERROR
	cmovns rax, qword [0xdf3]
	cmovns rax, qword [0xe17]
	cmovns r11w, r14w
	cmovns r14d, r10d
	cmovns r10, r9
	cmovns r10w, r15w, r14w
	cmovns r21w, r30w
	cmovns r23d, r29d
	cmovns r29, r31
	cmovns r25w, r29w, r24w
	cmovns rsi, qword [eax+1]
	cmovns rax, qword [eax+64]
	cmovns rdi, rdi, qword [eax+1]
	cmovns rbx, rax, qword [eax+64]
	cmovns rax, [0x47b]
	cmovns rax, rsi, [0xd82]
%endif
