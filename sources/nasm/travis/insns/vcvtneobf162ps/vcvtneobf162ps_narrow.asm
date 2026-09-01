	vcvtneobf162ps xmm2, oword [0x5f3]
	vcvtneobf162ps xmm7, oword [0x9cd]
	vcvtneobf162ps ymm4, yword [0x664]
	vcvtneobf162ps ymm5, yword [0x838]

%ifdef ERROR
	vcvtneobf162ps xmm9, oword [0x1ce]
	vcvtneobf162ps ymm10, yword [0xb43]
%endif
