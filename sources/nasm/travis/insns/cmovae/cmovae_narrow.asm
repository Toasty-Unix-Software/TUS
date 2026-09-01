	cmovae bx, bp
	cmovae di, word [0xd54]
	cmovae edi, ebp
	cmovae edi, edx
	cmovae dx, ax, cx
	cmovae cx, di, word [0x9ae]

%ifdef ERROR
	cmovae rsi, rax
	cmovae rax, rdx
	cmovae r14w, r11w
	cmovae r11d, r9d
	cmovae r12, r13
	cmovae r10w, r13w, r13w
	cmovae r31w, r28w
	cmovae r25d, r18d
	cmovae r23, r19
	cmovae r29w, r20w, r28w
	cmovae rdx, qword [eax+1]
	cmovae rbx, qword [eax+64]
	cmovae rax, rdx, qword [eax+1]
	cmovae rsi, rax, qword [eax+64]
	cmovae rsi, [0x350]
	cmovae rcx, rbx, [0x3f4]
%endif
