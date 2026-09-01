	mulx ebx, eax, dword [0x9b9]
	mulx ebx, dword [0x9b9]
	mulx esi, edx, ecx

%ifdef ERROR
	mulx rdx, rcx, rcx
	mulx rdx, rcx
	mulx rbx, rdi, rbp
	mulx r14d, r12d, r14d
	mulx r8, r9, r14
	mulx r19d, r20d, r17d
	mulx r22, r18, r21
	mulx rbx, rdi, qword [eax+1]
	mulx rbp, rsi, qword [eax+64]
	mulx rbp, rdx, [0x812]
%endif
