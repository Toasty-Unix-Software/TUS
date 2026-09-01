	bt ax, di
	bt ax, dx
	bt dword [0xf36], ebx
	bt ebp, ebp
	bt cx, 0x6f
	bt word [0x900], 0x2e

%ifdef ERROR
	bt rbx, rsi
	bt rdi, rbx
	bt r15w, r9w
	bt r10d, r14d
	bt r8, r13
	bt r8w, 0x13
	bt r28w, r27w
	bt r22d, r16d
	bt r21, r25
	bt r17w, 0xd9
	bt qword [eax+1], rsi
	bt qword [eax+64], rbx
	bt [0xef2], rax
%endif
