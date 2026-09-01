	lgs bp, word [0x128]
	lgs cx, word [0x68d]
	lgs eax, dword [0x835]
	lgs edi, dword [0x57c]

%ifdef ERROR
	lgs rcx, qword [0x7d0]
	lgs rsi, qword [0xd67]
	lgs r8w, word [0x1f6]
	lgs r13d, dword [0xcba]
	lgs r9, qword [0xe07]
	lgs r31w, word [0x240]
	lgs r16d, dword [0x113]
	lgs r23, qword [0xc15]
	lgs rcx, qword [eax+1]
	lgs rax, qword [eax+64]
	lgs rcx, [0x641]
%endif
