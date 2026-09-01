	cvtsi2ss xmm3, dword [0xd27]
	cvtsi2ss xmm7, ecx
	cvtsi2ss xmm0, rdi
	cvtsi2ss xmm7, qword [0x4b3]

%ifdef ERROR
	cvtsi2ss xmm11, r14d
	cvtsi2ss xmm13, r13
%endif
