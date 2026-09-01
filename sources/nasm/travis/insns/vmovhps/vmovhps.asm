default rel
	vmovhps xmm1, xmm6, qword [0xbf1]
	vmovhps xmm1, qword [0xbf1]
	vmovhps xmm4, xmm7, qword [0xcaf]
	vmovhps qword [0x5c7], xmm4
	vmovhps qword [0x58f], xmm4
	vmovhps xmm11, xmm10, qword [0xc13]
	vmovhps qword [0x41a], xmm14
	vmovhps xmm24, xmm26, qword [0xec3]
	vmovhps qword [0x1a3], xmm18
	vmovhps xmm5, xmm7, qword [eax+1]
	vmovhps xmm5, xmm3, qword [eax+64]
	vmovhps qword [eax+1], xmm3
	vmovhps qword [eax+64], xmm6
	vmovhps xmm3, xmm5, [0x62c]
	vmovhps [0x165], xmm3
