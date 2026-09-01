	vcvtsd2ss xmm4, xmm7, xmm0
	vcvtsd2ss xmm4, xmm0
	vcvtsd2ss xmm3, xmm2, xmm5
	vcvtsd2ss xmm2, xmm6, xmm3
	vcvtsd2ss xmm2, xmm3
	vcvtsd2ss xmm0, xmm5, xmm1

%ifdef ERROR
	vcvtsd2ss xmm9, xmm12, xmm9
	vcvtsd2ss xmm10, xmm10, xmm8
	vcvtsd2ss xmm19, xmm24, xmm26
	vcvtsd2ss xmm20, xmm31, xmm21
%endif
