default rel
	vpmaskmovq xmm7, xmm2, oword [0xb18]
	vpmaskmovq xmm7, oword [0xb18]
	vpmaskmovq xmm1, xmm6, oword [0x2f6]
	vpmaskmovq ymm1, ymm7, yword [0x58f]
	vpmaskmovq ymm1, yword [0x58f]
	vpmaskmovq ymm7, ymm5, yword [0x1bd]
	vpmaskmovq oword [0x20c], xmm6, xmm6
	vpmaskmovq oword [0x206], xmm7, xmm0
	vpmaskmovq yword [0x93f], ymm3, ymm6
	vpmaskmovq yword [0xcce], ymm4, ymm0
	vpmaskmovq xmm10, xmm14, oword [0x36b]
	vpmaskmovq ymm13, ymm15, yword [0x7aa]
	vpmaskmovq oword [0x40c], xmm10, xmm8
	vpmaskmovq yword [0xf36], ymm14, ymm8
	vpmaskmovq xmm0, xmm4, oword [eax+1]
	vpmaskmovq xmm2, xmm4, oword [eax+64]
	vpmaskmovq ymm5, ymm4, yword [eax+1]
	vpmaskmovq ymm2, ymm6, yword [eax+64]
	vpmaskmovq oword [eax+1], xmm2, xmm5
	vpmaskmovq oword [eax+64], xmm6, xmm6
	vpmaskmovq yword [eax+1], ymm7, ymm2
	vpmaskmovq yword [eax+64], ymm7, ymm2
	vpmaskmovq xmm1, xmm2, [0xda3]
	vpmaskmovq ymm1, ymm4, [0xa75]
	vpmaskmovq [0xcd5], xmm1, xmm0
	vpmaskmovq [0x3dd], ymm5, ymm4
