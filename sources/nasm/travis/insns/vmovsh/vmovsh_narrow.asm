	vmovsh xmm2, word [0xb2c]
	vmovsh xmm5, word [0xd4f]
	vmovsh word [0x932], xmm2
	vmovsh word [0xc57], xmm1
	vmovsh xmm1, xmm3, xmm6
	vmovsh xmm1, xmm6
	vmovsh xmm4, xmm0, xmm3

%ifdef ERROR
	vmovsh xmm14, word [0x606]
	vmovsh word [0x6dd], xmm11
	vmovsh xmm10, xmm15, xmm8
	vmovsh xmm29, word [0xf4a]
	vmovsh word [0x156], xmm18
	vmovsh xmm17, xmm16, xmm24
%endif
