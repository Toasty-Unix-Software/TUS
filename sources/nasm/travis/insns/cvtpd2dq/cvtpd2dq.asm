default rel
	cvtpd2dq xmm1, xmm5
	cvtpd2dq xmm7, xmm5
	cvtpd2dq xmm12, xmm9
	cvtpd2dq xmm3, oword [eax+1]
	cvtpd2dq xmm2, oword [eax+64]
	cvtpd2dq xmm4, [0xa6e]
