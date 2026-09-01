default rel
	cvttps2dq xmm4, xmm2
	cvttps2dq xmm3, oword [0x7d4]
	cvttps2dq xmm11, xmm10
	cvttps2dq xmm5, oword [eax+1]
	cvttps2dq xmm2, oword [eax+64]
	cvttps2dq xmm5, [0x6fa]
