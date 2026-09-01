	inc si
	inc cx
	inc ebx
	inc ecx
	inc cl
	inc byte [0xcee]
	inc si
	inc si

%ifdef ERROR
	inc r13w
	inc r10d
	inc r11b
	inc r11w
	inc r30w
	inc r21d
	inc r27b
	inc r19w
	inc rcx, qword [eax+1]
	inc rax, qword [eax+64]
	inc rbp, [0xf3d]
%endif
