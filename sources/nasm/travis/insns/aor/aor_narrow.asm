	aor dword [0xfb0], edi
	aor dword [0xf62], ebp

%ifdef ERROR
	aor qword [0x81c], rcx
	aor qword [0x1a8], rdx
	aor dword [0xcfa], r12d
	aor qword [0xe8a], r12
	aor dword [0x5e7], r17d
	aor qword [0xee8], r19
	aor qword [eax+1], rdx
	aor qword [eax+64], rdi
	aor [0xca7], rcx
%endif
