	vextracti64x2 xmm7, ymm4, 0xee
	vextracti64x2 xmm7, ymm6, 0x61
	vextracti64x2 xmm1, zmm7, 0xdc
	vextracti64x2 xmm1, zmm3, 0x8f
	vextracti64x2 oword [0xabf], ymm5, 0x1c
	vextracti64x2 oword [0xad8], ymm2, 0xbd
	vextracti64x2 oword [0x33b], zmm3, 0x22
	vextracti64x2 oword [0x2b2], zmm2, 0xa4

%ifdef ERROR
	vextracti64x2 xmm10, ymm11, 0x49
	vextracti64x2 xmm11, zmm12, 0x90
	vextracti64x2 oword [0xd89], ymm14, 0x91
	vextracti64x2 oword [0x229], zmm15, 0x30
	vextracti64x2 xmm29, ymm18, 0x79
	vextracti64x2 xmm16, zmm26, 0xc1
	vextracti64x2 oword [0x23f], ymm23, 0xa9
	vextracti64x2 oword [0x298], zmm17, 0x38
%endif
