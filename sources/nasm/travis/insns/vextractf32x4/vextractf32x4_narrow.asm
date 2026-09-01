	vextractf32x4 xmm7, ymm1, 0x3e
	vextractf32x4 xmm6, ymm7, 0x66
	vextractf32x4 xmm2, zmm2, 0x54
	vextractf32x4 xmm7, zmm1, 0x8b
	vextractf32x4 oword [0x12b], ymm3, 0x74
	vextractf32x4 oword [0x2c2], ymm2, 0xe6
	vextractf32x4 oword [0x6fa], zmm2, 0x4c
	vextractf32x4 oword [0x821], zmm2, 0x47

%ifdef ERROR
	vextractf32x4 xmm8, ymm15, 0x29
	vextractf32x4 xmm14, zmm9, 0xdf
	vextractf32x4 oword [0xdc9], ymm15, 0xf8
	vextractf32x4 oword [0x271], zmm13, 0x9
	vextractf32x4 xmm16, ymm24, 0x58
	vextractf32x4 xmm24, zmm24, 0x75
	vextractf32x4 oword [0xb25], ymm27, 0xe9
	vextractf32x4 oword [0xe22], zmm19, 0x94
%endif
