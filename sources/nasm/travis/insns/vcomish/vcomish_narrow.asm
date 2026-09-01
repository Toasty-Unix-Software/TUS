	vcomish xmm6, xmm0
	vcomish xmm5, word [0xd90]

%ifdef ERROR
	vcomish xmm10, xmm11
	vcomish xmm29, xmm28
%endif
