default rel
	rcr byte [0x905], 1
	rcr byte [0xcbc], 1
	rcr di, 1
	rcr bp, 1
	rcr dword [0xbb8], 1
	rcr eax, 1
	rcr rbp, 1
	rcr rax, 1
	rcr r12b, 1
	rcr r15w, 1
	rcr r10d, 1
	rcr r13, 1
	rcr r18b, 1
	rcr r28w, 1
	rcr r16d, 1
	rcr r30, 1
	rcr byte [eax+1], 1
	rcr byte [eax+64], 1
	rcr word [eax+1], 1
	rcr word [eax+64], 1
	rcr dword [eax+1], 1
	rcr dword [eax+64], 1
	rcr qword [eax+1], 1
	rcr qword [eax+64], 1
	rcr byte [eax+1], cl
	rcr byte [eax+64], cl
	rcr word [eax+1], cl
	rcr word [eax+64], cl
	rcr dword [eax+1], cl
	rcr dword [eax+64], cl
	rcr qword [eax+1], cl
	rcr qword [eax+64], cl
	rcr byte [eax+1], cx
	rcr byte [eax+64], cx
	rcr word [eax+1], cx
	rcr word [eax+64], cx
	rcr dword [eax+1], cx
	rcr dword [eax+64], cx
	rcr qword [eax+1], cx
	rcr qword [eax+64], cx
	rcr byte [eax+1], ecx
	rcr byte [eax+64], ecx
	rcr word [eax+1], ecx
	rcr word [eax+64], ecx
	rcr dword [eax+1], ecx
	rcr dword [eax+64], ecx
	rcr qword [eax+1], ecx
	rcr qword [eax+64], ecx
	rcr byte [eax+1], 0x9
	rcr byte [eax+64], 0x8c
	rcr word [eax+1], 0xbd
	rcr word [eax+64], 0xb2
	rcr dword [eax+1], 0x5d
	rcr dword [eax+64], 0xb5
	rcr qword [eax+1], 0x57
	rcr qword [eax+64], 0x8e
	rcr al, byte [eax+1], 1
	rcr dl, byte [eax+64], 1
	rcr si, word [eax+1], 1
	rcr bx, word [eax+64], 1
	rcr edx, dword [eax+1], 1
	rcr edx, dword [eax+64], 1
	rcr rsi, qword [eax+1], 1
	rcr rsi, qword [eax+64], 1
	rcr al, byte [eax+1], cl
	rcr cl, byte [eax+64], cl
	rcr dx, word [eax+1], cl
	rcr dx, word [eax+64], cl
	rcr edi, dword [eax+1], cl
	rcr edx, dword [eax+64], cl
	rcr rbp, qword [eax+1], cl
	rcr rdi, qword [eax+64], cl
	rcr bl, byte [eax+1], cx
	rcr al, byte [eax+64], cx
	rcr ax, word [eax+1], cx
	rcr bp, word [eax+64], cx
	rcr ebp, dword [eax+1], cx
	rcr edx, dword [eax+64], cx
	rcr rdx, qword [eax+1], cx
	rcr rax, qword [eax+64], cx
	rcr al, byte [eax+1], ecx
	rcr dl, byte [eax+64], ecx
	rcr bp, word [eax+1], ecx
	rcr bp, word [eax+64], ecx
	rcr eax, dword [eax+1], ecx
	rcr ebx, dword [eax+64], ecx
	rcr rbx, qword [eax+1], ecx
	rcr rdi, qword [eax+64], ecx
	rcr bl, byte [eax+1], 0x40
	rcr cl, byte [eax+64], 0xc6
	rcr dx, word [eax+1], 0x35
	rcr cx, word [eax+64], 0x8c
	rcr ebx, dword [eax+1], 0x93
	rcr edi, dword [eax+64], 0xaa
	rcr rdx, qword [eax+1], 0x69
	rcr rsi, qword [eax+64], 0xb7
	rcr [0x6d8], 1
	rcr [0xd53], 1
	rcr [0x4d2], 1
	rcr [0x155], 1
	rcr [0x411], cl
	rcr [0x71d], cl
	rcr [0x113], cl
	rcr [0x8f0], cl
	rcr [0x375], cx
	rcr [0xce6], cx
	rcr [0x14c], cx
	rcr [0x87b], cx
	rcr [0xe22], ecx
	rcr [0x260], ecx
	rcr [0x901], ecx
	rcr [0x8f4], ecx
	rcr [0xe93], 0x57
	rcr [0x264], 0x90
	rcr [0xbd8], 0xce
	rcr [0xc14], 0x14
	rcr al, [0x785], 1
	rcr dx, [0xa93], 1
	rcr ecx, [0xe5f], 1
	rcr rcx, [0x648], 1
	rcr bl, [0x91c], cl
	rcr si, [0x7a6], cl
	rcr edx, [0x89b], cl
	rcr rax, [0xafc], cl
	rcr bl, [0x98a], cx
	rcr dx, [0xb43], cx
	rcr ecx, [0x272], cx
	rcr rax, [0x5cf], cx
	rcr bl, [0xe92], ecx
	rcr si, [0x637], ecx
	rcr ebx, [0x31b], ecx
	rcr rbp, [0xa24], ecx
	rcr cl, [0xa13], 0xe1
	rcr bx, [0x327], 0xab
	rcr ebp, [0xe56], 0x9c
	rcr rax, [0xbae], 0x9c
