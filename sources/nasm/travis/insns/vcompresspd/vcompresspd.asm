default rel
	vcompresspd oword [0x7aa], xmm4
	vcompresspd oword [0x740], xmm7
	vcompresspd yword [0x90a], ymm4
	vcompresspd yword [0xcc5], ymm2
	vcompresspd zword [0x599], zmm2
	vcompresspd zword [0xf60], zmm1
	vcompresspd xmm1, xmm1
	vcompresspd xmm6, xmm0
	vcompresspd oword [0x8cf], xmm11
	vcompresspd yword [0x9e9], ymm9
	vcompresspd zword [0xea0], zmm13
	vcompresspd xmm15, xmm14
	vcompresspd oword [0x1f5], xmm25
	vcompresspd yword [0xc0c], ymm26
	vcompresspd zword [0x6e8], zmm22
	vcompresspd xmm23, xmm19
	vcompresspd oword [0x2c6]{k2}, xmm2
	vcompresspd yword [0x32b]{k3}, ymm5
	vcompresspd zword [0x47f]{k6}, zmm3
	vcompresspd xmm7{k3}, xmm1
	vcompresspd ymm2{k7}, ymm0
	vcompresspd zmm4{k3}, zmm6
	vcompresspd oword [eax+1], xmm2
	vcompresspd oword [eax+64], xmm7
	vcompresspd yword [eax+1], ymm5
	vcompresspd yword [eax+64], ymm6
	vcompresspd zword [eax+1], zmm0
	vcompresspd zword [eax+64], zmm0
	vcompresspd [0xe7d], xmm1
	vcompresspd [0x68d], ymm0
	vcompresspd [0xfdd], zmm3
