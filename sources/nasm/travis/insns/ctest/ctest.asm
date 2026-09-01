default rel
	ctest 10, bl, al
	ctest 0, al, al
	ctest 12, word [0xb81], bx
	ctest 7, dx, dx
	ctest 10, dword [0x6ea], edx
	ctest 4, ebp, edi
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
	ctest 2, byte [eax+1], bl
	ctest 11, byte [eax+64], dl
	ctest 10, word [eax+1], ax
	ctest 4, word [eax+64], ax
	ctest 6, dword [eax+1], ecx
	ctest 0, dword [eax+64], edi
	ctest 6, qword [eax+1], rcx
	ctest 9, qword [eax+64], rdx
	ctest 8, byte [eax+1], 0x83
	ctest 12, byte [eax+64], 0x5a
	ctest 4, word [eax+1], 0xdacb
	ctest 15, word [eax+64], 0xa235
	ctest 8, dword [eax+1], 0x2f8e7fc5
	ctest 11, dword [eax+64], 0x1d967033
	ctest 8, qword [eax+1], 189310555
	ctest 3, qword [eax+64], -524833596
	ctest 4, [0x74c], cl
	ctest 14, [0xd2c], si
	ctest 9, [0xb5c], edx
	ctest 0, [0x8a4], rbx
	ctest 0, [0x9dd], 0xb
	ctest 2, [0x96f], 0x57fa
	ctest 11, [0xa6c], 0x363caf5e
	ctest 6, [0x1fd], 318894437
