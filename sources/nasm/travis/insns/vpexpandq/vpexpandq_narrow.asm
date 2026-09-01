	vpexpandq xmm5, oword [0x200]
	vpexpandq xmm0, oword [0x562]
	vpexpandq ymm7, yword [0x11a]
	vpexpandq ymm4, yword [0x2a2]
	vpexpandq zmm6, zword [0x9e0]
	vpexpandq zmm2, zword [0x3ff]
	vpexpandq xmm6, xmm3
	vpexpandq xmm3, xmm3

%ifdef ERROR
	vpexpandq xmm9, oword [0xbbd]
	vpexpandq ymm8, yword [0x28c]
	vpexpandq zmm9, zword [0xde7]
	vpexpandq xmm15, xmm15
	vpexpandq xmm27, oword [0x69b]
	vpexpandq ymm19, yword [0xeda]
	vpexpandq zmm17, zword [0x4ff]
	vpexpandq xmm24, xmm17
%endif
