	vmread dword [0xd4f], esi
	vmread edi, ebp

%ifdef ERROR
	vmread rdi, rcx
	vmread qword [0x49c], rbp
%endif
