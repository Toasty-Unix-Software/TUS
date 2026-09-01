	cvtsi2sd xmm5, dword [0x1ae]
	cvtsi2sd xmm1, dword [0xcac]
	cvtsi2sd xmm3, rdi
	cvtsi2sd xmm5, rax

%ifdef ERROR
	cvtsi2sd xmm8, r14d
	cvtsi2sd xmm14, r14
%endif
