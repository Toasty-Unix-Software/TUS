	bsf ax, ax
	bsf si, word [0xe75]
	bsf ecx, ecx
	bsf esi, dword [0xfb8]

%ifdef ERROR
	bsf rdi, rcx
	bsf rbx, rax
	bsf r8w, r14w
	bsf r14d, r8d
	bsf r10, r8
	bsf r22w, r21w
	bsf r31d, r30d
	bsf r26, r29
	bsf rcx, qword [eax+1]
	bsf rcx, qword [eax+64]
	bsf rdi, [0xe8f]
%endif
