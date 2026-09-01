	vextractf32x8 ymm2, zmm4, 0xb0
	vextractf32x8 ymm2, zmm0, 0x5e
	vextractf32x8 yword [0xb36], zmm2, 0xc8
	vextractf32x8 yword [0x610], zmm0, 0x42

%ifdef ERROR
	vextractf32x8 ymm12, zmm15, 0x86
	vextractf32x8 yword [0x683], zmm9, 0x1e
	vextractf32x8 ymm17, zmm31, 0xcd
	vextractf32x8 yword [0x666], zmm24, 0x9d
%endif
