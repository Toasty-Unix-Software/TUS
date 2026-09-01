	vmovlpd xmm7, xmm4, qword [0xd8e]
	vmovlpd xmm7, qword [0xd8e]
	vmovlpd xmm2, xmm0, qword [0xff0]
	vmovlpd qword [0x7a3], xmm0
	vmovlpd qword [0x5f7], xmm6

%ifdef ERROR
	vmovlpd xmm14, xmm9, qword [0x128]
	vmovlpd qword [0xff6], xmm10
	vmovlpd xmm27, xmm25, qword [0x3b4]
	vmovlpd qword [0x360], xmm21
%endif
