	ud2
	ud2
	ud2 di, word [0xa90]
	ud2 ax, word [0xba0]
	ud2 ecx, ebp
	ud2 esi, edx

%ifdef ERROR
	ud2 rbp, rdx
	ud2 rbp, qword [0x824]
	ud2 r11w, r10w
	ud2 r10d, r14d
	ud2 r10, r8
	ud2 r17w, r17w
	ud2 r26d, r24d
	ud2 r17, r23
	ud2 rbx, qword [eax+1]
	ud2 rdx, qword [eax+64]
	ud2 rdi, [0xfe3]
%endif
