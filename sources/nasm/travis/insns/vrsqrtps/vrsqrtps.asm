default rel
	vrsqrtps xmm1, xmm5
	vrsqrtps xmm3, oword [0xbf7]
	vrsqrtps ymm7, yword [0xa63]
	vrsqrtps ymm2, yword [0x8fd]
	vrsqrtps xmm8, xmm8
	vrsqrtps ymm8, ymm9
	vrsqrtps xmm3, oword [eax+1]
	vrsqrtps xmm3, oword [eax+64]
	vrsqrtps ymm0, yword [eax+1]
	vrsqrtps ymm1, yword [eax+64]
	vrsqrtps xmm5, [0xb21]
	vrsqrtps ymm0, [0x23d]
