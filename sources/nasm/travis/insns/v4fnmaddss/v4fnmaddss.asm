default rel
	v4fnmaddss xmm0, xmm1, [0x81d]
	v4fnmaddss xmm2, xmm2, [0xfbe]
	v4fnmaddss xmm11, xmm9, [0x55f]
	v4fnmaddss xmm31, xmm22, [0x86e]
	v4fnmaddss xmm0{k4}, xmm4, [0x491]
	v4fnmaddss xmm5{k3}{z}, xmm2, [0x125]
	v4fnmaddss xmm7, xmm5, [eax+1]
	v4fnmaddss xmm7, xmm2, [eax+64]
