	vfpclassps k1, xmm1, 0x39
	vfpclassps k2, oword [0xff3], 0xc3
	vfpclassps k5, yword [0x2d4], 0x8f
	vfpclassps k6, ymm2, 0x2c
	vfpclassps k2, zword [0xda9], 0xcb
	vfpclassps k5, zmm2, 0x61

%ifdef ERROR
	vfpclassps k5, xmm15, 0x15
	vfpclassps k6, ymm12, 0xe7
	vfpclassps k3, zmm15, 0x9
	vfpclassps k1, xmm16, 0xb3
	vfpclassps k6, ymm27, 0x57
	vfpclassps k3, zmm29, 0xda
%endif
