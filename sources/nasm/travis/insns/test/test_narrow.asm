	test cl, dl
	test cl, al
	test dx, si
	test dx, di
	test dword [0x934], edx
	test esi, ebp

%ifdef ERROR
	test qword [0x9e0], rdi
	test rax, rax
	test r14b, r15b
	test r10w, r8w
	test r12d, r14d
	test r11, r8
	test r23b, r27b
	test r22w, r28w
	test r17d, r23d
	test r28, r16
	test qword [eax+1], rdi
	test qword [eax+64], rax
	test rdx, qword [eax+1]
	test rdx, qword [eax+64]
	test qword [eax+1], 174016346
	test qword [eax+64], -197546387
	test [0x6a9], rbp
	test rsi, [0xa80]
	test [0xd04], -6445748
%endif
