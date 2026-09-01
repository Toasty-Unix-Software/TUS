	vpsrlvd xmm6, xmm6, xmm2
	vpsrlvd xmm6, xmm2
	vpsrlvd xmm4, xmm3, xmm0
	vpsrlvd ymm1, ymm6, ymm5
	vpsrlvd ymm1, ymm5
	vpsrlvd ymm7, ymm6, yword [0x742]
	vpsrlvd xmm6, xmm6, xmm5
	vpsrlvd xmm6, xmm5
	vpsrlvd xmm3, xmm5, xmm1
	vpsrlvd ymm3, ymm1, yword [0xafe]
	vpsrlvd ymm3, yword [0xafe]
	vpsrlvd ymm4, ymm4, ymm4

%ifdef ERROR
	vpsrlvd xmm14, xmm15, xmm14
	vpsrlvd ymm11, ymm9, ymm11
	vpsrlvd xmm10, xmm11, xmm8
	vpsrlvd ymm9, ymm13, ymm13
	vpsrlvd xmm21, xmm26, xmm19
	vpsrlvd ymm17, ymm31, ymm25
	vpsrlvd xmm22, xmm20, xmm21
	vpsrlvd ymm19, ymm23, ymm25
%endif
