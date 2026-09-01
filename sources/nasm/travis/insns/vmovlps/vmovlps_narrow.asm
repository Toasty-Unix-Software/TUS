	vmovlps xmm4, xmm4, qword [0x3de]
	vmovlps xmm4, qword [0x3de]
	vmovlps xmm5, xmm0, qword [0x52d]
	vmovlps qword [0x1fd], xmm4
	vmovlps qword [0x67b], xmm1

%ifdef ERROR
	vmovlps xmm10, xmm12, qword [0x6ac]
	vmovlps qword [0x8ac], xmm14
	vmovlps xmm20, xmm20, qword [0x165]
	vmovlps qword [0x5ca], xmm31
%endif
