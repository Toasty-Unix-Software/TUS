default rel
	kmov k3, k4
	kmov k2, k6
	kmov k5, k7
	kmov k1, k5
	kmov k1, k6
	kmov k5, k4
	kmov k3, k2
	kmov k5, k5
	kmov byte [eax+1], k2
	kmov byte [eax+64], k2
	kmov word [eax+1], k5
	kmov word [eax+64], k7
	kmov dword [eax+1], k3
	kmov dword [eax+64], k6
	kmov qword [eax+1], k2
	kmov qword [eax+64], k3
	kmov [0x4ad], k5
	kmov [0x555], k2
	kmov [0xbee], k3
	kmov [0x612], k2
