	ccmp 6, byte [0xe6d], dl
	ccmp 9, byte [0xb44], dl
	ccmp 6, si, dx
	ccmp 1, word [0x7aa], ax
	ccmp 3, ecx, ebp
	ccmp 2, dword [0xf62], esi

%ifdef ERROR
	ccmp 13, qword [0xbe1], rbx
	ccmp 15, rdx, rbx
	ccmp 10, r11b, r13b
	ccmp 3, r13w, r13w
	ccmp 0, r11d, r9d
	ccmp 0, r15, r8
	ccmp 0, r17b, r16b
	ccmp 9, r27w, r19w
	ccmp 6, r24d, r28d
	ccmp 1, r27, r22
	ccmp 7, qword [eax+1], rax
	ccmp 3, qword [eax+64], rbx
	ccmp 10, rdi, qword [eax+1]
	ccmp 9, rcx, qword [eax+64]
	ccmp 4, qword [eax+1], 125
	ccmp 9, qword [eax+64], -1
	ccmp 1, qword [eax+1], 350346
	ccmp 10, qword [eax+64], -222208413
	ccmp 6, [0xa23], rax
	ccmp 14, rdx, [0x16a]
	ccmp 9, [0x9e3], 8
	ccmp 4, [0x28d], -393521394
%endif
