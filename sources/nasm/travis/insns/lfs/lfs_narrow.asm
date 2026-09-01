	lfs di, word [0xe2a]
	lfs di, word [0xa97]
	lfs ebx, dword [0x9db]
	lfs ecx, dword [0x793]

%ifdef ERROR
	lfs rcx, qword [0x339]
	lfs rdx, qword [0xce2]
	lfs r10w, word [0xc5d]
	lfs r14d, dword [0x3e7]
	lfs r8, qword [0x636]
	lfs r23w, word [0x106]
	lfs r28d, dword [0x911]
	lfs r17, qword [0xce2]
	lfs rax, qword [eax+1]
	lfs rax, qword [eax+64]
	lfs rcx, [0x364]
%endif
