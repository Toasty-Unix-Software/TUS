default rel
	cmpxchg byte [0xca0], bl
	cmpxchg al, al
	cmpxchg bp, dx
	cmpxchg ax, di
	cmpxchg eax, edx
	cmpxchg ecx, ecx
	cmpxchg qword [0x590], rsi
	cmpxchg qword [0xc80], rcx
	cmpxchg r9b, r14b
	cmpxchg r15w, r10w
	cmpxchg r14d, r10d
	cmpxchg r11, r14
	cmpxchg r18b, r25b
	cmpxchg r31w, r26w
	cmpxchg r22d, r31d
	cmpxchg r21, r17
	cmpxchg byte [eax+1], bl
	cmpxchg byte [eax+64], bl
	cmpxchg word [eax+1], bp
	cmpxchg word [eax+64], si
	cmpxchg dword [eax+1], esi
	cmpxchg dword [eax+64], esi
	cmpxchg qword [eax+1], rax
	cmpxchg qword [eax+64], rax
	cmpxchg [0x935], al
	cmpxchg [0xa23], si
	cmpxchg [0x292], esi
	cmpxchg [0x6cc], rcx
