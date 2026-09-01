	ctestc 10, bl, al
	ctestc 12, bl, al
	ctestc 15, dx, ax
	ctestc 14, word [0xe16], bx
	ctestc 2, esi, edi
	ctestc 4, ebp, edx

%ifdef ERROR
	ctestc 14, rcx, rax
	ctestc 1, qword [0x570], rcx
	ctestc 2, r9b, r8b
	ctestc 13, r15w, r8w
	ctestc 5, r11d, r15d
	ctestc 12, r8, r12
	ctestc 6, r28b, r23b
	ctestc 7, r21w, r24w
	ctestc 9, r20d, r27d
	ctestc 0, r18, r30
	ctestc 3, qword [eax+1], rsi
	ctestc 5, qword [eax+64], rcx
	ctestc 2, qword [eax+1], -346250776
	ctestc 12, qword [eax+64], 437496154
	ctestc 7, [0x2ad], rsi
	ctestc 10, [0x9a8], 349171076
%endif
