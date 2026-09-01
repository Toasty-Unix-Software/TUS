	ccmpb 11, bl, bl
	ccmpb 11, cl, dl
	ccmpb 3, bp, dx
	ccmpb 5, cx, cx
	ccmpb 13, esi, edx
	ccmpb 13, ebp, ebx

%ifdef ERROR
	ccmpb 11, rbx, rdx
	ccmpb 13, qword [0xbd5], rdi
	ccmpb 11, r8b, r12b
	ccmpb 0, r11w, r9w
	ccmpb 4, r12d, r12d
	ccmpb 13, r13, r9
	ccmpb 1, r29b, r24b
	ccmpb 7, r31w, r16w
	ccmpb 14, r17d, r27d
	ccmpb 15, r17, r17
	ccmpb 8, qword [eax+1], rsi
	ccmpb 13, qword [eax+64], rax
	ccmpb 0, rbp, qword [eax+1]
	ccmpb 11, rsi, qword [eax+64]
	ccmpb 14, qword [eax+1], -75
	ccmpb 4, qword [eax+64], 64
	ccmpb 9, qword [eax+1], -409014020
	ccmpb 10, qword [eax+64], 345437304
	ccmpb 1, [0xb25], rsi
	ccmpb 6, rsi, [0x668]
	ccmpb 12, [0x1c0], -43
	ccmpb 10, [0xcdd], 34792225
%endif
