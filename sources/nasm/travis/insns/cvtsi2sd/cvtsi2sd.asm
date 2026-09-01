default rel
	cvtsi2sd xmm5, dword [0x1ae]
	cvtsi2sd xmm1, dword [0xcac]
	cvtsi2sd xmm3, rdi
	cvtsi2sd xmm5, rax
	cvtsi2sd xmm8, r14d
	cvtsi2sd xmm14, r14
	cvtsi2sd xmm2, dword [eax+1]
	cvtsi2sd xmm3, dword [eax+64]
	cvtsi2sd xmm5, qword [eax+1]
	cvtsi2sd xmm6, qword [eax+64]
	cvtsi2sd xmm3, [0x899]
	cvtsi2sd xmm1, [0xb32]
