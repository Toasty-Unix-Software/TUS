	vcvttss2usis edi, xmm5
	vcvttss2usis edx, dword [0x12d]

%ifdef ERROR
	vcvttss2usis rax, xmm2
	vcvttss2usis rdi, xmm4
	vcvttss2usis r11d, xmm9
	vcvttss2usis r9, xmm8
	vcvttss2usis r30d, xmm24
	vcvttss2usis r24, xmm29
	vcvttss2usis rdi, dword [eax+1]
	vcvttss2usis rax, dword [eax+64]
	vcvttss2usis rbx, [0x98b]
%endif
