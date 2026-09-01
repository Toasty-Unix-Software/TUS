	shr al, 1
	shr bl, 1
	shr dx, 1
	shr bx, 1
	shr ecx, 1
	shr dword [0xe47], 1
	shr rax, 1
	shr rbx, 1

%ifdef ERROR
	shr r13b, 1
	shr r10w, 1
	shr r8d, 1
	shr r11, 1
	shr r19b, 1
	shr r24w, 1
	shr r18d, 1
	shr r18, 1
%endif
