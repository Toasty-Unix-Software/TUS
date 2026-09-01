default rel
	mpsadbw xmm4, oword [0x4f8], 0x1a
	mpsadbw xmm1, xmm6, 0x44
	mpsadbw xmm15, xmm9, 0xf6
	mpsadbw xmm7, oword [eax+1], 0xb9
	mpsadbw xmm0, oword [eax+64], 0x75
	mpsadbw xmm4, [0xcaa], 0xd1
