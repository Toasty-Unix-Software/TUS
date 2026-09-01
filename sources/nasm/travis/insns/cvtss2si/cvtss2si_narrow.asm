	cvtss2si ebx, xmm7
	cvtss2si eax, xmm0

%ifdef ERROR
	cvtss2si rax, xmm3
	cvtss2si rdx, dword [0x9be]
	cvtss2si r13d, xmm10
	cvtss2si r11, xmm12
	cvtss2si rcx, dword [eax+1]
	cvtss2si rdx, dword [eax+64]
	cvtss2si rcx, [0x53f]
%endif
