default rel
	vpcmpistrm xmm6, xmm0, 0x5f
	vpcmpistrm xmm0, xmm5, 0x7c
	vpcmpistrm xmm10, xmm10, 0x1e
	vpcmpistrm xmm5, oword [eax+1], 0x48
	vpcmpistrm xmm4, oword [eax+64], 0x4c
	vpcmpistrm xmm2, [0x5e3], 0xd6
