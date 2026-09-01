	mpsadbw xmm4, oword [0x4f8], 0x1a
	mpsadbw xmm1, xmm6, 0x44

%ifdef ERROR
	mpsadbw xmm15, xmm9, 0xf6
%endif
