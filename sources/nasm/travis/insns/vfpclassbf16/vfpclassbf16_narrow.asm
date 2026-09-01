	vfpclassbf16 k1, xmm6, 0x82
	vfpclassbf16 k3, oword [0xb95], 0x6a
	vfpclassbf16 k1, ymm7, 0xd8
	vfpclassbf16 k2, ymm6, 0xc5
	vfpclassbf16 k7, zmm5, 0xf6
	vfpclassbf16 k2, zmm4, 0x4a

%ifdef ERROR
	vfpclassbf16 k4, xmm12, 0x27
	vfpclassbf16 k5, ymm10, 0xcb
	vfpclassbf16 k5, zmm9, 0x63
	vfpclassbf16 k1, xmm17, 0xc7
	vfpclassbf16 k6, ymm22, 0x65
	vfpclassbf16 k6, zmm24, 0xe7
%endif
