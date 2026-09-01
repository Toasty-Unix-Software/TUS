default rel
	vpcmpestrm xmm3, oword [0x4ff], 0xf5
	vpcmpestrm xmm3, xmm5, 0x71
	vpcmpestrm xmm12, xmm14, 0xe9
	vpcmpestrm xmm1, oword [eax+1], 0xd7
	vpcmpestrm xmm0, oword [eax+64], 0x92
	vpcmpestrm xmm6, [0xa74], 0x59
