	hint_nop 0x6d, ax, dx
	hint_nop 0x2, bp, bp
	hint_nop 0x4c, ebp, ebx
	hint_nop 0x6a, ecx, dword [0x6cf]

%ifdef ERROR
	hint_nop 0xc, rbx, qword [0x403]
	hint_nop 0x37, rax, rdx
	hint_nop 0x7b, r8w, r14w
	hint_nop 0x7, r14d, r14d
	hint_nop 0x61, r8, r10
	hint_nop 0x32, r23w, r22w
	hint_nop 0x45, r16d, r16d
	hint_nop 0x64, r31, r16
	hint_nop 0x23, rbp, qword [eax+1]
	hint_nop 0x2b, rsi, qword [eax+64]
	hint_nop 0xc, rcx, [0xf45]
%endif
