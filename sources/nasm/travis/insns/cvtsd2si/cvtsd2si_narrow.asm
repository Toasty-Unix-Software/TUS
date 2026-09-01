	cvtsd2si esi, xmm0
	cvtsd2si esi, qword [0x7c1]

%ifdef ERROR
	cvtsd2si rbx, xmm7
	cvtsd2si rdi, qword [0x2d8]
	cvtsd2si r9d, xmm12
	cvtsd2si r8, xmm10
	cvtsd2si rdx, qword [eax+1]
	cvtsd2si rax, qword [eax+64]
	cvtsd2si rsi, [0x781]
%endif
