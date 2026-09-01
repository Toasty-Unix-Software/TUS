default rel
	vpexpandq xmm5, oword [0x200]
	vpexpandq xmm0, oword [0x562]
	vpexpandq ymm7, yword [0x11a]
	vpexpandq ymm4, yword [0x2a2]
	vpexpandq zmm6, zword [0x9e0]
	vpexpandq zmm2, zword [0x3ff]
	vpexpandq xmm6, xmm3
	vpexpandq xmm3, xmm3
	vpexpandq xmm9, oword [0xbbd]
	vpexpandq ymm8, yword [0x28c]
	vpexpandq zmm9, zword [0xde7]
	vpexpandq xmm15, xmm15
	vpexpandq xmm27, oword [0x69b]
	vpexpandq ymm19, yword [0xeda]
	vpexpandq zmm17, zword [0x4ff]
	vpexpandq xmm24, xmm17
	vpexpandq xmm0{k4}, oword [0x855]
	vpexpandq ymm6{k4}, yword [0x3e2]
	vpexpandq zmm4{k1}, zword [0xf58]
	vpexpandq xmm4{k4}, xmm2
	vpexpandq ymm4{k6}, ymm2
	vpexpandq zmm4{k1}, zmm4
	vpexpandq xmm6{k2}{z}, oword [0x2e2]
	vpexpandq ymm0{k5}{z}, yword [0x177]
	vpexpandq zmm6{k2}{z}, zword [0xac3]
	vpexpandq xmm2{k5}{z}, xmm1
	vpexpandq ymm6{k7}{z}, ymm0
	vpexpandq zmm4{k2}{z}, zmm6
	vpexpandq xmm3, oword [eax+1]
	vpexpandq xmm6, oword [eax+64]
	vpexpandq ymm7, yword [eax+1]
	vpexpandq ymm6, yword [eax+64]
	vpexpandq zmm4, zword [eax+1]
	vpexpandq zmm0, zword [eax+64]
	vpexpandq xmm4, [0x7d4]
	vpexpandq ymm4, [0x248]
	vpexpandq zmm6, [0x130]
