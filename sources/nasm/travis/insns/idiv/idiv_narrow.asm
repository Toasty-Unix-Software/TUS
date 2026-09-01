	idiv bl
	idiv al
	idiv cx
	idiv word [0x445]
	idiv edx
	idiv dword [0xd5d]
	idiv rax
	idiv rdx

%ifdef ERROR
	idiv r10b
	idiv r14w
	idiv r13d
	idiv r15
	idiv r21b
	idiv r24w
	idiv r28d
	idiv r27
%endif
