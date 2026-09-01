	vextracti32x8 ymm7, zmm1, 0x49
	vextracti32x8 ymm5, zmm0, 0x68
	vextracti32x8 yword [0xaaf], zmm4, 0x43
	vextracti32x8 yword [0xfe0], zmm2, 0xd1

%ifdef ERROR
	vextracti32x8 ymm9, zmm11, 0xc2
	vextracti32x8 yword [0x3cc], zmm14, 0x43
	vextracti32x8 ymm19, zmm24, 0x9b
	vextracti32x8 yword [0x910], zmm29, 0x50
%endif
