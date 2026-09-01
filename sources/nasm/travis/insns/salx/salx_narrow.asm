	salx ebx, ebx, cl
	salx ebx, cl
	salx ecx, ebp, dl
	salx edi, dword [0xb57], si
	salx edi, si
	salx ecx, edx, bp
	salx edi, ecx, ebp
	salx edi, ebp
	salx eax, dword [0x20e], ebp

%ifdef ERROR
	salx ebp, eax, rbx
	salx ebp, rbx
	salx ebx, ecx, rdx
	salx r10d, r14d, r14b
	salx r14d, r15d, r10w
	salx r12d, r15d, r10d
	salx r9d, r14d, r11
	salx r16d, r23d, r29b
	salx r17d, r29d, r23w
	salx r21d, r24d, r20d
	salx r30d, r20d, r16
	salx ecx, dword [eax+1], rbx
	salx eax, dword [eax+64], rbp
	salx rcx, qword [eax+1], dl
	salx rcx, qword [eax+64], al
	salx rdi, qword [eax+1], bp
	salx rax, qword [eax+64], dx
	salx rbx, qword [eax+1], esi
	salx rsi, qword [eax+64], edi
	salx rax, qword [eax+1], rax
	salx rcx, qword [eax+64], rbx
	salx edx, [0xce1], rbx
	salx rdi, [0x2ac], cl
	salx rbp, [0x4db], dx
	salx rbp, [0x60d], edx
	salx rax, [0x9ac], rdi
%endif
