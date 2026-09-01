	seto byte [0x857]
	seto dl
	seto edi
	seto eax

%ifdef ERROR
	seto rdx
	seto rdi
	seto r10b
	seto r11
	seto r14d
	seto r21b
	seto r21
	seto r27d
%endif
