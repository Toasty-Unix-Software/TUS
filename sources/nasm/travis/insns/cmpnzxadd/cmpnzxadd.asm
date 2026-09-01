default rel
	cmpnzxadd dword [0x6ba], ecx, ebp
	cmpnzxadd dword [0x85f], ebx, ebp
	cmpnzxadd qword [0x289], rsi, rcx
	cmpnzxadd qword [0xe17], rbx, rax
	cmpnzxadd dword [0x47b], r15d, r9d
	cmpnzxadd qword [0x10d], r11, r13
	cmpnzxadd dword [0x553], r27d, r19d
	cmpnzxadd qword [0x639], r30, r18
	cmpnzxadd dword [eax+1], eax, ebx
	cmpnzxadd dword [eax+64], edx, edx
	cmpnzxadd qword [eax+1], rbp, rax
	cmpnzxadd qword [eax+64], rsi, rbx
	cmpnzxadd [0xcd7], edx, ebp
	cmpnzxadd [0xffe], rsi, rsi
