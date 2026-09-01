	cmovg si, bp
	cmovg bp, si
	cmovg ebx, edi
	cmovg ecx, ecx
	cmovg si, si, di
	cmovg si, dx, word [0x8b0]

%ifdef ERROR
	cmovg rax, rsi
	cmovg rax, rax
	cmovg r9w, r10w
	cmovg r9d, r10d
	cmovg r15, r14
	cmovg r10w, r15w, r9w
	cmovg r16w, r17w
	cmovg r23d, r27d
	cmovg r31, r16
	cmovg r17w, r20w, r29w
	cmovg rbp, qword [eax+1]
	cmovg rcx, qword [eax+64]
	cmovg rax, rcx, qword [eax+1]
	cmovg rcx, rsi, qword [eax+64]
	cmovg rdi, [0x786]
	cmovg rdi, rdi, [0xb99]
%endif
