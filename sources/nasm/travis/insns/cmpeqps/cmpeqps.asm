default rel
	cmpeqps xmm3, xmm0
	cmpeqps xmm3, xmm5
	cmpeqps xmm10, xmm11
	cmpeqps xmm6, oword [eax+1]
	cmpeqps xmm4, oword [eax+64]
	cmpeqps xmm0, [0x8a4]
