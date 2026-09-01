default rel
	dpps xmm6, oword [0xdff], 0xe0
	dpps xmm7, oword [0xe49], 0xbe
	dpps xmm10, xmm14, 0xd9
	dpps xmm7, oword [eax+1], 0x4e
	dpps xmm6, oword [eax+64], 0xb4
	dpps xmm0, [0xc14], 0x88
