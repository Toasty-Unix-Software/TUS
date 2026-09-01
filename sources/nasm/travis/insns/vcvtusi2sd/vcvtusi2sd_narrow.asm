	vcvtusi2sd xmm6, xmm7, dword [0xdbb]
	vcvtusi2sd xmm6, dword [0xdbb]
	vcvtusi2sd xmm4, xmm5, dword [0x608]
	vcvtusi2sd xmm3, xmm7, rbp
	vcvtusi2sd xmm3, rbp
	vcvtusi2sd xmm1, xmm0, rcx

%ifdef ERROR
	vcvtusi2sd xmm10, xmm14, r12d
	vcvtusi2sd xmm13, xmm10, r8
	vcvtusi2sd xmm19, xmm17, r27d
	vcvtusi2sd xmm16, xmm17, r16
%endif
