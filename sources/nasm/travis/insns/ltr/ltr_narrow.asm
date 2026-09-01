	ltr word [0x3d7]
	ltr word [0x313]
	ltr bx
	ltr bp
	ltr ebp
	ltr eax

%ifdef ERROR
	ltr rdx
	ltr rsi
	ltr r15w
	ltr r15d
	ltr r12
	ltr r18w
	ltr r28d
	ltr r26
%endif
