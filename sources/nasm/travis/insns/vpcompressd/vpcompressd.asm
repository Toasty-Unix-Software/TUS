default rel
	vpcompressd oword [0x707], xmm2
	vpcompressd oword [0x766], xmm4
	vpcompressd yword [0xf0f], ymm6
	vpcompressd yword [0xcba], ymm0
	vpcompressd zword [0x876], zmm6
	vpcompressd zword [0xab2], zmm6
	vpcompressd xmm4, xmm7
	vpcompressd xmm5, xmm7
	vpcompressd oword [0x23a], xmm12
	vpcompressd yword [0x258], ymm12
	vpcompressd zword [0x193], zmm12
	vpcompressd xmm11, xmm12
	vpcompressd oword [0x90c], xmm21
	vpcompressd yword [0xc19], ymm27
	vpcompressd zword [0x14d], zmm23
	vpcompressd xmm23, xmm22
	vpcompressd oword [0x3d3]{k7}, xmm2
	vpcompressd yword [0x67c]{k5}, ymm6
	vpcompressd zword [0xc96]{k5}, zmm6
	vpcompressd xmm1{k5}, xmm5
	vpcompressd ymm3{k4}, ymm7
	vpcompressd zmm6{k2}, zmm4
	vpcompressd oword [eax+1], xmm2
	vpcompressd oword [eax+64], xmm3
	vpcompressd yword [eax+1], ymm6
	vpcompressd yword [eax+64], ymm5
	vpcompressd zword [eax+1], zmm2
	vpcompressd zword [eax+64], zmm7
	vpcompressd [0x652], xmm1
	vpcompressd [0x9c1], ymm1
	vpcompressd [0x735], zmm2
