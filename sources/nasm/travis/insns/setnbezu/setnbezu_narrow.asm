	setnbezu esi
	setnbezu eax
	setnbezu al
	setnbezu bl

%ifdef ERROR
	setnbezu rdi
	setnbezu rbx
	setnbezu r8
	setnbezu r13d
	setnbezu r14b
	setnbezu r28
	setnbezu r31d
	setnbezu r30b
%endif
