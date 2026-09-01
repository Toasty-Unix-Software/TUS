	adcx edi, ebp
	adcx ebp, edi
	adcx ebp, esi, eax
	adcx esi, eax
	adcx eax, esi, ecx

%ifdef ERROR
	adcx rdi, rax
	adcx rdi, rax
	adcx rsi, rbx, rdi
	adcx rbx, rdi
	adcx rdx, rdx, qword [0x715]
	adcx r13d, r14d
	adcx r11, r12
	adcx r9d, r11d, r14d
	adcx r8, r12, r9
	adcx r17d, r29d
	adcx r17, r29
	adcx r18d, r26d, r16d
	adcx r20, r23, r31
	adcx rdi, qword [eax+1]
	adcx rbx, qword [eax+64]
	adcx rdi, rax, qword [eax+1]
	adcx rax, rbx, qword [eax+64]
	adcx rbx, [0x3eb]
	adcx rdx, rdx, [0xffe]
%endif
