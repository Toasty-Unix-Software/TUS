	vmovhpd xmm2, xmm2, qword [0x348]
	vmovhpd xmm2, qword [0x348]
	vmovhpd xmm5, xmm4, qword [0x56e]
	vmovhpd qword [0xc49], xmm1
	vmovhpd qword [0x22a], xmm3

%ifdef ERROR
	vmovhpd xmm14, xmm13, qword [0xa71]
	vmovhpd qword [0xbc6], xmm11
	vmovhpd xmm25, xmm18, qword [0xf90]
	vmovhpd qword [0xfce], xmm30
%endif
