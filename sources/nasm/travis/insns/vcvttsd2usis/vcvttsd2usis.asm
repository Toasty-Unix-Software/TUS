default rel
	vcvttsd2usis ebx, xmm6
	vcvttsd2usis ebp, qword [0xd29]
	vcvttsd2usis rbp, qword [0x9c1]
	vcvttsd2usis rcx, xmm1
	vcvttsd2usis r11d, xmm10
	vcvttsd2usis r15, xmm8
	vcvttsd2usis r17d, xmm16
	vcvttsd2usis r31, xmm30
	vcvttsd2usis eax, xmm2, {sae}
	vcvttsd2usis rdi, xmm4, {sae}
	vcvttsd2usis esi, qword [eax+1]
	vcvttsd2usis ecx, qword [eax+64]
	vcvttsd2usis rcx, qword [eax+1]
	vcvttsd2usis rax, qword [eax+64]
	vcvttsd2usis ebp, [0x88a]
	vcvttsd2usis rdi, [0xda9]
