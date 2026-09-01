default rel
	vcvtusi2ss xmm7, xmm4, dword [0x75b]
	vcvtusi2ss xmm7, dword [0x75b]
	vcvtusi2ss xmm2, xmm5, edx
	vcvtusi2ss xmm3, xmm2, qword [0x12e]
	vcvtusi2ss xmm3, qword [0x12e]
	vcvtusi2ss xmm2, xmm2, rbp
	vcvtusi2ss xmm10, xmm9, r11d
	vcvtusi2ss xmm8, xmm9, r11
	vcvtusi2ss xmm22, xmm24, r31d
	vcvtusi2ss xmm24, xmm22, r30
	vcvtusi2ss xmm0, xmm0, ecx, {rn-sae}
	vcvtusi2ss xmm3, xmm4, rbx, {rn-sae}
	vcvtusi2ss xmm3, xmm2, dword [eax+1]
	vcvtusi2ss xmm7, xmm0, dword [eax+64]
	vcvtusi2ss xmm3, xmm1, qword [eax+1]
	vcvtusi2ss xmm6, xmm1, qword [eax+64]
	vcvtusi2ss xmm6, xmm6, [0x6e6]
	vcvtusi2ss xmm3, xmm0, [0x749]
