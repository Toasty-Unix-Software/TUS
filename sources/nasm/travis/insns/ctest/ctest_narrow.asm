	ctest 10, bl, al
	ctest 0, al, al
	ctest 12, word [0xb81], bx
	ctest 7, dx, dx
	ctest 10, dword [0x6ea], edx
	ctest 4, ebp, edi

%ifdef ERROR
	ctest 6, rsi, rsi
	ctest 6, rbx, rsi
	ctest 15, r14b, r10b
	ctest 15, r15w, r8w
	ctest 9, r13d, r13d
	ctest 8, r13, r13
	ctest 13, r26b, r17b
	ctest 7, r16w, r21w
	ctest 8, r30d, r19d
	ctest 10, r26, r18
	ctest 6, qword [eax+1], rcx
	ctest 9, qword [eax+64], rdx
	ctest 8, qword [eax+1], 189310555
	ctest 3, qword [eax+64], -524833596
	ctest 0, [0x8a4], rbx
	ctest 6, [0x1fd], 318894437
%endif
