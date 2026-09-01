	vpcompressd oword [0x707], xmm2
	vpcompressd oword [0x766], xmm4
	vpcompressd yword [0xf0f], ymm6
	vpcompressd yword [0xcba], ymm0
	vpcompressd zword [0x876], zmm6
	vpcompressd zword [0xab2], zmm6
	vpcompressd xmm4, xmm7
	vpcompressd xmm5, xmm7

%ifdef ERROR
	vpcompressd oword [0x23a], xmm12
	vpcompressd yword [0x258], ymm12
	vpcompressd zword [0x193], zmm12
	vpcompressd xmm11, xmm12
	vpcompressd oword [0x90c], xmm21
	vpcompressd yword [0xc19], ymm27
	vpcompressd zword [0x14d], zmm23
	vpcompressd xmm23, xmm22
%endif
