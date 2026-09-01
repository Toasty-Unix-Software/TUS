default rel
	vreducess xmm4, xmm4, xmm1, 0xfc
	vreducess xmm4, xmm1, 0xfc
	vreducess xmm7, xmm0, dword [0xeca], 0xcd
	vreducess xmm11, xmm11, xmm12, 0x2b
	vreducess xmm24, xmm17, xmm20, 0x68
	vreducess xmm0{k2}, xmm5, dword [0x6e0], 0xcc
	vreducess xmm2{k6}{z}, xmm2, dword [0x78a], 0x65
	vreducess xmm3, xmm1, dword [eax+1], 0xc4
	vreducess xmm3, xmm3, dword [eax+64], 0xf8
	vreducess xmm4, xmm5, [0xcbf], 0x71
