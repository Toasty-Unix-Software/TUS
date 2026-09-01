	vmovq xmm3, xmm0
	vmovq xmm1, qword [0x2ba]
	vmovq qword [0x5d1], xmm5
	vmovq qword [0x79e], xmm3
	vmovq xmm0, qword [0x7c5]
	vmovq xmm2, rbx
	vmovq rax, xmm4
	vmovq qword [0xaa4], xmm2

%ifdef ERROR
	vmovq xmm13, xmm12
	vmovq xmm15, xmm8
	vmovq xmm11, r11
	vmovq r9, xmm12
	vmovq xmm30, xmm21
	vmovq xmm25, xmm20
	vmovq xmm27, r28
	vmovq r19, xmm24
%endif
