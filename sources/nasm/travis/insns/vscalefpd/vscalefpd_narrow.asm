	vscalefpd xmm6, xmm0, xmm7
	vscalefpd xmm6, xmm7
	vscalefpd xmm2, xmm7, xmm3
	vscalefpd ymm4, ymm0, ymm5
	vscalefpd ymm4, ymm5
	vscalefpd ymm1, ymm3, ymm7
	vscalefpd zmm1, zmm3, zmm1
	vscalefpd zmm1, zmm1
	vscalefpd zmm2, zmm4, zword [0x396]

%ifdef ERROR
	vscalefpd xmm11, xmm10, xmm8
	vscalefpd ymm13, ymm15, ymm10
	vscalefpd zmm8, zmm12, zmm13
	vscalefpd xmm22, xmm22, xmm22
	vscalefpd ymm24, ymm20, ymm26
	vscalefpd zmm27, zmm18, zmm24
%endif
