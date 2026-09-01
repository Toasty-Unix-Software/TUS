default rel
	vpexpandd xmm1, oword [0xe3c]
	vpexpandd xmm0, oword [0x72a]
	vpexpandd ymm6, yword [0xb05]
	vpexpandd ymm6, yword [0x807]
	vpexpandd zmm5, zword [0xbfb]
	vpexpandd zmm1, zword [0x557]
	vpexpandd xmm5, xmm2
	vpexpandd xmm5, xmm3
	vpexpandd xmm13, oword [0x95d]
	vpexpandd ymm8, yword [0xa9a]
	vpexpandd zmm13, zword [0xbb3]
	vpexpandd xmm14, xmm12
	vpexpandd xmm18, oword [0x6f2]
	vpexpandd ymm26, yword [0xdbd]
	vpexpandd zmm27, zword [0xadc]
	vpexpandd xmm20, xmm28
	vpexpandd xmm4{k4}, oword [0x22c]
	vpexpandd ymm1{k7}, yword [0x1c7]
	vpexpandd zmm2{k6}, zword [0xa38]
	vpexpandd xmm7{k6}, xmm2
	vpexpandd ymm2{k7}, ymm3
	vpexpandd zmm3{k6}, zmm4
	vpexpandd xmm2{k6}{z}, oword [0x5e5]
	vpexpandd ymm5{k3}{z}, yword [0x78f]
	vpexpandd zmm0{k5}{z}, zword [0x1c1]
	vpexpandd xmm2{k3}{z}, xmm6
	vpexpandd ymm6{k2}{z}, ymm1
	vpexpandd zmm6{k6}{z}, zmm1
	vpexpandd xmm4, oword [eax+1]
	vpexpandd xmm4, oword [eax+64]
	vpexpandd ymm1, yword [eax+1]
	vpexpandd ymm3, yword [eax+64]
	vpexpandd zmm2, zword [eax+1]
	vpexpandd zmm7, zword [eax+64]
	vpexpandd xmm0, [0x417]
	vpexpandd ymm0, [0x45a]
	vpexpandd zmm0, [0xa58]
