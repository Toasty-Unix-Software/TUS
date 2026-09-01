	vcvtusi2sh xmm7, xmm0, ebx
	vcvtusi2sh xmm7, ebx
	vcvtusi2sh xmm5, xmm2, edi
	vcvtusi2sh xmm2, xmm5, rbp
	vcvtusi2sh xmm2, rbp
	vcvtusi2sh xmm1, xmm3, qword [0x83f]

%ifdef ERROR
	vcvtusi2sh xmm8, xmm10, r11d
	vcvtusi2sh xmm13, xmm10, r12
	vcvtusi2sh xmm19, xmm22, r24d
	vcvtusi2sh xmm25, xmm16, r19
%endif
