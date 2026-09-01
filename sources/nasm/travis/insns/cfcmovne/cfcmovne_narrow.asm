	cfcmovne bp, di
	cfcmovne word [0x566], bp
	cfcmovne dword [0xfcc], ebx
	cfcmovne dword [0xcfe], eax
	cfcmovne di, ax
	cfcmovne ax, bx

%ifdef ERROR
	cfcmovne rsi, rdx
	cfcmovne rsi, rdx
	cfcmovne r11w, r8w
	cfcmovne r11d, r11d
	cfcmovne r9, r11
	cfcmovne r12w, r13w
	cfcmovne r27w, r17w
	cfcmovne r27d, r30d
	cfcmovne r21, r22
	cfcmovne r19w, r25w
	cfcmovne qword [eax+1], rax
	cfcmovne qword [eax+64], rcx
	cfcmovne rcx, qword [eax+1]
	cfcmovne rsi, qword [eax+64]
	cfcmovne rbp, rdx, qword [eax+1]
	cfcmovne rbp, rdi, qword [eax+64]
	cfcmovne [0x6bf], rdx
	cfcmovne rbp, [0x645]
	cfcmovne rbp, rdx, [0x726]
%endif
