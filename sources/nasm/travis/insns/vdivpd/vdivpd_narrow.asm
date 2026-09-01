	vdivpd xmm5, xmm4, oword [0xbec]
	vdivpd xmm5, oword [0xbec]
	vdivpd xmm5, xmm0, oword [0x9b3]
	vdivpd ymm1, ymm7, ymm2
	vdivpd ymm1, ymm2
	vdivpd ymm1, ymm0, ymm7
	vdivpd xmm0, xmm1, oword [0xd15]
	vdivpd xmm0, oword [0xd15]
	vdivpd xmm5, xmm0, oword [0x86c]
	vdivpd ymm4, ymm0, yword [0x3b3]
	vdivpd ymm4, yword [0x3b3]
	vdivpd ymm5, ymm3, ymm6

%ifdef ERROR
	vdivpd xmm14, xmm11, xmm8
	vdivpd ymm8, ymm14, ymm11
	vdivpd xmm9, xmm9, xmm14
	vdivpd ymm14, ymm8, ymm14
	vdivpd xmm30, xmm31, xmm20
	vdivpd ymm30, ymm22, ymm29
	vdivpd xmm23, xmm20, xmm22
	vdivpd ymm20, ymm19, ymm21
%endif
