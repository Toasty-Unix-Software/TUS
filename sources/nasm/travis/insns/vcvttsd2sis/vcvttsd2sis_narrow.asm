	vcvttsd2sis ecx, xmm5
	vcvttsd2sis ebp, xmm1

%ifdef ERROR
	vcvttsd2sis rbp, qword [0x614]
	vcvttsd2sis rbx, xmm6
	vcvttsd2sis r8d, xmm8
	vcvttsd2sis r12, xmm10
	vcvttsd2sis r21d, xmm27
	vcvttsd2sis r25, xmm22
	vcvttsd2sis rbx, qword [eax+1]
	vcvttsd2sis rbp, qword [eax+64]
	vcvttsd2sis rbp, [0x61b]
%endif
