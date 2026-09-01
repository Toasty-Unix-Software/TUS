default rel
	vexpandps xmm6, oword [0x459]
	vexpandps xmm7, oword [0x26e]
	vexpandps ymm3, yword [0x3a1]
	vexpandps ymm2, yword [0xaa0]
	vexpandps zmm3, zword [0xb15]
	vexpandps zmm2, zword [0x7c8]
	vexpandps xmm6, xmm2
	vexpandps xmm7, xmm6
	vexpandps xmm10, oword [0xf6c]
	vexpandps ymm9, yword [0xcbe]
	vexpandps zmm10, zword [0xa0f]
	vexpandps xmm14, xmm9
	vexpandps xmm17, oword [0xd91]
	vexpandps ymm31, yword [0x4d5]
	vexpandps zmm20, zword [0xd67]
	vexpandps xmm23, xmm30
	vexpandps xmm0{k7}, oword [0x29e]
	vexpandps ymm6{k1}, yword [0x929]
	vexpandps zmm6{k3}, zword [0xdee]
	vexpandps xmm3{k2}, xmm2
	vexpandps ymm3{k4}, ymm6
	vexpandps zmm3{k2}, zmm7
	vexpandps xmm5{k6}{z}, oword [0xe24]
	vexpandps ymm1{k3}{z}, yword [0x19b]
	vexpandps zmm3{k7}{z}, zword [0x9a6]
	vexpandps xmm6{k4}{z}, xmm0
	vexpandps ymm2{k5}{z}, ymm1
	vexpandps zmm3{k1}{z}, zmm3
	vexpandps xmm5, oword [eax+1]
	vexpandps xmm6, oword [eax+64]
	vexpandps ymm4, yword [eax+1]
	vexpandps ymm4, yword [eax+64]
	vexpandps zmm2, zword [eax+1]
	vexpandps zmm5, zword [eax+64]
	vexpandps xmm3, [0xf7e]
	vexpandps ymm2, [0x150]
	vexpandps zmm6, [0xe0b]
