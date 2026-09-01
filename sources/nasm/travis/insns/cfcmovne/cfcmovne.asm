default rel
	cfcmovne bp, di
	cfcmovne word [0x566], bp
	cfcmovne dword [0xfcc], ebx
	cfcmovne dword [0xcfe], eax
	cfcmovne rsi, rdx
	cfcmovne rsi, rdx
	cfcmovne di, ax
	cfcmovne ax, bx
	cfcmovne r11w, r8w
	cfcmovne r11d, r11d
	cfcmovne r9, r11
	cfcmovne r12w, r13w
	cfcmovne r27w, r17w
	cfcmovne r27d, r30d
	cfcmovne r21, r22
	cfcmovne r19w, r25w
	cfcmovne word [eax+1], bx
	cfcmovne word [eax+64], cx
	cfcmovne dword [eax+1], eax
	cfcmovne dword [eax+64], eax
	cfcmovne qword [eax+1], rax
	cfcmovne qword [eax+64], rcx
	cfcmovne bx, word [eax+1]
	cfcmovne cx, word [eax+64]
	cfcmovne ebx, dword [eax+1]
	cfcmovne ebp, dword [eax+64]
	cfcmovne rcx, qword [eax+1]
	cfcmovne rsi, qword [eax+64]
	cfcmovne si, si, word [eax+1]
	cfcmovne ax, ax, word [eax+64]
	cfcmovne edx, esi, dword [eax+1]
	cfcmovne ebx, edi, dword [eax+64]
	cfcmovne rbp, rdx, qword [eax+1]
	cfcmovne rbp, rdi, qword [eax+64]
	cfcmovne [0x13a], cx
	cfcmovne [0x5d3], ecx
	cfcmovne [0x6bf], rdx
	cfcmovne bp, [0x429]
	cfcmovne ecx, [0x5e5]
	cfcmovne rbp, [0x645]
	cfcmovne ax, bp, [0xfd6]
	cfcmovne edi, edi, [0xf8e]
	cfcmovne rbp, rdx, [0x726]
