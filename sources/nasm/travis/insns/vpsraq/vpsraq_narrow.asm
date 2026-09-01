	vpsraq xmm5, xmm4, xmm3
	vpsraq xmm5, xmm3
	vpsraq xmm2, xmm4, oword [0x1b8]
	vpsraq ymm7, ymm2, xmm2
	vpsraq ymm7, xmm2
	vpsraq ymm7, ymm3, xmm7
	vpsraq zmm1, zmm5, xmm6
	vpsraq zmm1, xmm6
	vpsraq zmm1, zmm3, xmm2
	vpsraq xmm5, xmm2, 0xf9
	vpsraq xmm5, 0xf9
	vpsraq xmm3, xmm6, 0xa2

%ifdef ERROR
	vpsraq xmm8, xmm10, xmm13
	vpsraq ymm15, ymm8, xmm15
	vpsraq zmm10, zmm13, xmm14
	vpsraq xmm10, xmm15, 0x53
	vpsraq xmm23, xmm30, xmm30
	vpsraq ymm20, ymm25, xmm28
	vpsraq zmm31, zmm19, xmm16
	vpsraq xmm19, xmm25, 0x4a
%endif
