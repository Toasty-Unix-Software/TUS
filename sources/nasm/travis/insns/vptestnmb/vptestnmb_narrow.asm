	vptestnmb k3, xmm4, oword [0x114]
	vptestnmb k5, xmm1, oword [0xd0a]
	vptestnmb k1, ymm7, ymm4
	vptestnmb k3, ymm4, yword [0x9fd]
	vptestnmb k6, zmm4, zmm7
	vptestnmb k4, zmm3, zword [0x9b7]

%ifdef ERROR
	vptestnmb k1, xmm13, xmm12
	vptestnmb k5, ymm10, ymm9
	vptestnmb k5, zmm12, zmm14
	vptestnmb k4, xmm30, xmm25
	vptestnmb k4, ymm20, ymm16
	vptestnmb k2, zmm20, zmm24
%endif
