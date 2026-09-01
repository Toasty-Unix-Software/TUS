	vexpandps xmm6, oword [0x459]
	vexpandps xmm7, oword [0x26e]
	vexpandps ymm3, yword [0x3a1]
	vexpandps ymm2, yword [0xaa0]
	vexpandps zmm3, zword [0xb15]
	vexpandps zmm2, zword [0x7c8]
	vexpandps xmm6, xmm2
	vexpandps xmm7, xmm6

%ifdef ERROR
	vexpandps xmm10, oword [0xf6c]
	vexpandps ymm9, yword [0xcbe]
	vexpandps zmm10, zword [0xa0f]
	vexpandps xmm14, xmm9
	vexpandps xmm17, oword [0xd91]
	vexpandps ymm31, yword [0x4d5]
	vexpandps zmm20, zword [0xd67]
	vexpandps xmm23, xmm30
%endif
