	or byte [0xa24], dl
	or bl, dl
	or bp, bx
	or word [0x916], si
	or dword [0xbba], esi
	or ebp, eax

%ifdef ERROR
	or qword [0x819], rdi
	or rbp, rbp
	or r14b, r11b
	or r13w, r8w
	or r13d, r8d
	or r15, r10
	or r29b, r17b
	or r22w, r17w
	or r25d, r25d
	or r23, r24
	or qword [eax+1], rbp
	or qword [eax+64], rcx
	or rdi, qword [eax+1]
	or rcx, qword [eax+64]
	or qword [eax+1], 4
	or qword [eax+64], 47
	or qword [eax+1], -222397584
	or qword [eax+64], 128390512
	or rsi, rdi, qword [eax+1]
	or rbp, rbp, qword [eax+64]
	or rax, qword [eax+1], rdx
	or rdi, qword [eax+64], rsi
	or rsi, qword [eax+1], 5
	or rdx, qword [eax+64], 118
	or rcx, qword [eax+1], -220663921
	or rsi, qword [eax+64], -288737155
	or [0x1d9], rdi
	or rdi, [0xc1e]
	or [0x487], 79
	or [0x2e6], 32597466
	or rcx, rax, [0xbcd]
	or rbx, [0x588], rax
	or rcx, [0xf55], 88
	or rsi, [0x278], -409843528
%endif
