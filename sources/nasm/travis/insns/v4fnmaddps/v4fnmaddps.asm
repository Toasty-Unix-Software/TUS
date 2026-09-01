default rel
	v4fnmaddps zmm7, zmm3, [0xbfa]
	v4fnmaddps zmm3, zmm1, [0xfd1]
	v4fnmaddps zmm11, zmm13, [0xd86]
	v4fnmaddps zmm16, zmm24, [0xdca]
	v4fnmaddps zmm5{k6}, zmm6, [0xe33]
	v4fnmaddps zmm6{k1}{z}, zmm1, [0x80d]
	v4fnmaddps zmm3, zmm7, [eax+1]
	v4fnmaddps zmm5, zmm0, [eax+64]
