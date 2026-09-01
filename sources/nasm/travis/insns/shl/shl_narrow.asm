	shl cl, 1
	shl byte [0xa1d], 1
	shl ax, 1
	shl bp, 1
	shl eax, 1
	shl dword [0x119], 1
	shl qword [0xbdd], 1
	shl rdi, 1

%ifdef ERROR
	shl r9b, 1
	shl r11w, 1
	shl r13d, 1
	shl r9, 1
	shl r19b, 1
	shl r23w, 1
	shl r27d, 1
	shl r28, 1
%endif
