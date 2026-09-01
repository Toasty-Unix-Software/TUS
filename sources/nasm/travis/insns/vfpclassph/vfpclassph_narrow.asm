	vfpclassph k6, xmm0, 0x18
	vfpclassph k1, xmm7, 0x35
	vfpclassph k1, ymm2, 0x43
	vfpclassph k7, ymm6, 0xec
	vfpclassph k6, zmm5, 0xdf
	vfpclassph k5, zmm2, 0x4c

%ifdef ERROR
	vfpclassph k6, xmm15, 0xb7
	vfpclassph k4, ymm11, 0xbc
	vfpclassph k4, zmm11, 0x7f
	vfpclassph k6, xmm21, 0xbb
	vfpclassph k6, ymm22, 0x7e
	vfpclassph k1, zmm26, 0xbc
%endif
