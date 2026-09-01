	crc32 ebx, byte [0x645]
	crc32 edx, bl
	crc32 ebx, ax
	crc32 ebp, di
	crc32 ecx, ecx
	crc32 eax, ecx
	crc32 ebp, rcx
	crc32 ecx, rdx

%ifdef ERROR
	crc32 r9d, r10b
	crc32 r13d, r11w
	crc32 r13d, r13d
	crc32 r10d, r9
	crc32 r27d, r19b
	crc32 r23d, r26w
	crc32 r26d, r25d
	crc32 r26d, r30
	crc32 rdi, byte [eax+1]
	crc32 rdx, byte [eax+64]
	crc32 rcx, word [eax+1]
	crc32 rbp, word [eax+64]
	crc32 rdi, dword [eax+1]
	crc32 rax, dword [eax+64]
	crc32 rax, qword [eax+1]
	crc32 rcx, qword [eax+64]
	crc32 rbp, [0x770]
	crc32 rdx, [0x402]
	crc32 rbp, [0xf7d]
	crc32 rbp, [0xb0e]
%endif
