	vrcpbf16 xmm5, xmm6
	vrcpbf16 xmm3, xmm5
	vrcpbf16 ymm4, yword [0xc86]
	vrcpbf16 ymm5, yword [0xbb4]
	vrcpbf16 zmm1, zmm3
	vrcpbf16 zmm3, zword [0x438]

%ifdef ERROR
	vrcpbf16 xmm15, xmm11
	vrcpbf16 ymm11, ymm12
	vrcpbf16 zmm8, zmm15
	vrcpbf16 xmm20, xmm24
	vrcpbf16 ymm25, ymm22
	vrcpbf16 zmm24, zmm16
%endif
