	vpsubb xmm6, xmm2, xmm1
	vpsubb xmm6, xmm1
	vpsubb xmm5, xmm5, oword [0xffc]
	vpsubb ymm4, ymm0, ymm5
	vpsubb ymm4, ymm5
	vpsubb ymm1, ymm2, ymm0
	vpsubb xmm3, xmm4, xmm0
	vpsubb xmm3, xmm0
	vpsubb xmm4, xmm4, xmm1
	vpsubb ymm4, ymm3, yword [0x87e]
	vpsubb ymm4, yword [0x87e]
	vpsubb ymm2, ymm3, ymm0

%ifdef ERROR
	vpsubb xmm8, xmm10, xmm8
	vpsubb ymm9, ymm10, ymm9
	vpsubb xmm9, xmm11, xmm10
	vpsubb ymm10, ymm10, ymm10
	vpsubb xmm24, xmm20, xmm24
	vpsubb ymm16, ymm28, ymm25
	vpsubb xmm24, xmm17, xmm25
	vpsubb ymm23, ymm21, ymm30
%endif
