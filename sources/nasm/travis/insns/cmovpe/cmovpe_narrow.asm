	cmovpe dx, ax
	cmovpe bx, bp
	cmovpe ebp, ecx
	cmovpe esi, dword [0xbeb]
	cmovpe di, si, word [0x144]
	cmovpe bx, ax, word [0x269]

%ifdef ERROR
	cmovpe rsi, rdi
	cmovpe rdx, rbx
	cmovpe r13w, r8w
	cmovpe r12d, r8d
	cmovpe r12, r8
	cmovpe r10w, r12w, r12w
	cmovpe r30w, r22w
	cmovpe r25d, r16d
	cmovpe r31, r24
	cmovpe r29w, r22w, r18w
	cmovpe rdx, qword [eax+1]
	cmovpe rdi, qword [eax+64]
	cmovpe rcx, rcx, qword [eax+1]
	cmovpe rax, rdi, qword [eax+64]
	cmovpe rcx, [0x137]
	cmovpe rdx, rdi, [0x11b]
%endif
