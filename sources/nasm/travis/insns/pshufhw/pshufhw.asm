default rel
	pshufhw xmm6, xmm6, 0x57
	pshufhw xmm4, xmm5, 0x58
	pshufhw xmm3, [0xcb8], 0x7c
	pshufhw xmm0, [0x62a], 0x61
	pshufhw xmm9, xmm8, 0x76
	pshufhw xmm8, [0xf14], 0x55
	pshufhw xmm7, [eax+1], 0x11
	pshufhw xmm0, [eax+64], 0x45
