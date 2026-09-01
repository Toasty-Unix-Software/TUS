	andn edx, esi, ebx
	andn edx, ebx
	andn ecx, esi, edi
	andn k6, k3, k7
	andn k6, k7
	andn k3, k2, k3
	andn k5, k4, k1
	andn k5, k1
	andn k1, k1, k7

%ifdef ERROR
	andn rcx, rbp, rcx
	andn rcx, rcx
	andn rbx, rbp, qword [0xc5c]
	andn r10d, r15d, r13d
	andn r14, r10, r15
	andn r19d, r18d, r28d
	andn r16, r21, r29
	andn rbp, rdx, qword [eax+1]
	andn rax, rcx, qword [eax+64]
	andn rdx, rcx, [0x49c]
%endif
