	vfmsubpd xmm4, xmm5, xmm0, xmm1
	vfmsubpd xmm4, xmm0, xmm1
	vfmsubpd xmm6, xmm0, oword [0x9bb], xmm6
	vfmsubpd ymm2, ymm7, ymm5, ymm7
	vfmsubpd ymm2, ymm5, ymm7
	vfmsubpd ymm4, ymm7, yword [0x799], ymm0
	vfmsubpd xmm7, xmm4, xmm1, oword [0x6e5]
	vfmsubpd xmm7, xmm1, oword [0x6e5]
	vfmsubpd xmm2, xmm0, xmm3, xmm3
	vfmsubpd ymm7, ymm6, ymm4, ymm7
	vfmsubpd ymm7, ymm4, ymm7
	vfmsubpd ymm4, ymm7, ymm4, ymm0

%ifdef ERROR
	vfmsubpd xmm12, xmm9, xmm15, xmm8
	vfmsubpd ymm14, ymm14, ymm14, ymm8
	vfmsubpd xmm8, xmm8, xmm10, xmm8
	vfmsubpd ymm10, ymm13, ymm13, ymm13
%endif
