	cvttss2si eax, dword [0x75a]
	cvttss2si esi, dword [0x7b6]

%ifdef ERROR
	cvttss2si rbp, dword [0xdc3]
	cvttss2si rsi, dword [0xf70]
	cvttss2si r8d, xmm14
	cvttss2si r8, xmm11
	cvttss2si rax, dword [eax+1]
	cvttss2si rdx, dword [eax+64]
	cvttss2si rbp, [0x459]
%endif
