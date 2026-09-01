	vcvtusi2ss xmm7, xmm4, dword [0x75b]
	vcvtusi2ss xmm7, dword [0x75b]
	vcvtusi2ss xmm2, xmm5, edx
	vcvtusi2ss xmm3, xmm2, qword [0x12e]
	vcvtusi2ss xmm3, qword [0x12e]
	vcvtusi2ss xmm2, xmm2, rbp

%ifdef ERROR
	vcvtusi2ss xmm10, xmm9, r11d
	vcvtusi2ss xmm8, xmm9, r11
	vcvtusi2ss xmm22, xmm24, r31d
	vcvtusi2ss xmm24, xmm22, r30
%endif
