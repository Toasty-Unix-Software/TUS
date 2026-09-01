	cmovb ax, word [0x81e]
	cmovb bp, di
	cmovb esi, dword [0xe43]
	cmovb ebx, ecx
	cmovb dx, bp, ax
	cmovb cx, di, cx

%ifdef ERROR
	cmovb rdi, rdx
	cmovb rbp, rcx
	cmovb r13w, r8w
	cmovb r13d, r9d
	cmovb r13, r10
	cmovb r12w, r12w, r10w
	cmovb r24w, r23w
	cmovb r23d, r20d
	cmovb r28, r19
	cmovb r24w, r26w, r29w
	cmovb rbx, qword [eax+1]
	cmovb rdx, qword [eax+64]
	cmovb rcx, rdx, qword [eax+1]
	cmovb rsi, rbp, qword [eax+64]
	cmovb rdx, [0x415]
	cmovb rdi, rsi, [0xb3f]
%endif
