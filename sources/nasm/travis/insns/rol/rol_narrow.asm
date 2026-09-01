	rol byte [0x39d], 1
	rol byte [0x91c], 1
	rol word [0xf53], 1
	rol ax, 1
	rol dword [0x537], 1
	rol edi, 1
	rol rsi, 1
	rol rdi, 1

%ifdef ERROR
	rol r8b, 1
	rol r13w, 1
	rol r12d, 1
	rol r12, 1
	rol r26b, 1
	rol r23w, 1
	rol r30d, 1
	rol r20, 1
	rol rdi, qword [eax+1], 1
	rol rbp, qword [eax+64], 1
	rol rdx, qword [eax+1], 1
	rol rbx, qword [eax+64], 1
	rol rdx, qword [eax+1], cl
	rol rbx, qword [eax+64], cl
	rol rbx, qword [eax+1], cx
	rol rdx, qword [eax+64], cx
	rol rax, qword [eax+1], ecx
	rol rax, qword [eax+64], ecx
	rol rdx, qword [eax+1], 0xc
	rol rbp, qword [eax+64], 0x87
	rol rdx, [0xb75], 1
	rol rsi, [0x3db], 1
	rol rcx, [0x9dc], cl
	rol rbp, [0xad4], cx
	rol rcx, [0x385], ecx
	rol rbp, [0xa3e], 0xfb
%endif
