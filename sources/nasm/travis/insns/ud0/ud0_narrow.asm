	ud0 di, word [0xb49]
	ud0 dx, di
	ud0 edi, dword [0x342]
	ud0 edx, ecx
	ud0
	ud0

%ifdef ERROR
	ud0 rbx, rcx
	ud0 rbp, rdx
	ud0 r9w, r12w
	ud0 r14d, r8d
	ud0 r12, r9
	ud0 r17w, r18w
	ud0 r31d, r17d
	ud0 r26, r20
	ud0 rcx, qword [eax+1]
	ud0 rcx, qword [eax+64]
	ud0 rsi, [0x65b]
%endif
