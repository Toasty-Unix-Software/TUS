default rel
	vmovlps xmm4, xmm4, qword [0x3de]
	vmovlps xmm4, qword [0x3de]
	vmovlps xmm5, xmm0, qword [0x52d]
	vmovlps qword [0x1fd], xmm4
	vmovlps qword [0x67b], xmm1
	vmovlps xmm10, xmm12, qword [0x6ac]
	vmovlps qword [0x8ac], xmm14
	vmovlps xmm20, xmm20, qword [0x165]
	vmovlps qword [0x5ca], xmm31
	vmovlps xmm2, xmm3, qword [eax+1]
	vmovlps xmm4, xmm7, qword [eax+64]
	vmovlps qword [eax+1], xmm1
	vmovlps qword [eax+64], xmm5
	vmovlps xmm6, xmm3, [0xb51]
	vmovlps [0x411], xmm1
