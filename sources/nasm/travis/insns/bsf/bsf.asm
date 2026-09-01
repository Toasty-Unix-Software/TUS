default rel
	bsf ax, ax
	bsf si, word [0xe75]
	bsf ecx, ecx
	bsf esi, dword [0xfb8]
	bsf rdi, rcx
	bsf rbx, rax
	bsf r8w, r14w
	bsf r14d, r8d
	bsf r10, r8
	bsf r22w, r21w
	bsf r31d, r30d
	bsf r26, r29
	bsf si, word [eax+1]
	bsf bx, word [eax+64]
	bsf esi, dword [eax+1]
	bsf ebx, dword [eax+64]
	bsf rcx, qword [eax+1]
	bsf rcx, qword [eax+64]
	bsf ax, [0x5f5]
	bsf eax, [0x5e8]
	bsf rdi, [0xe8f]
