	vcompresspd oword [0x7aa], xmm4
	vcompresspd oword [0x740], xmm7
	vcompresspd yword [0x90a], ymm4
	vcompresspd yword [0xcc5], ymm2
	vcompresspd zword [0x599], zmm2
	vcompresspd zword [0xf60], zmm1
	vcompresspd xmm1, xmm1
	vcompresspd xmm6, xmm0

%ifdef ERROR
	vcompresspd oword [0x8cf], xmm11
	vcompresspd yword [0x9e9], ymm9
	vcompresspd zword [0xea0], zmm13
	vcompresspd xmm15, xmm14
	vcompresspd oword [0x1f5], xmm25
	vcompresspd yword [0xc0c], ymm26
	vcompresspd zword [0x6e8], zmm22
	vcompresspd xmm23, xmm19
%endif
