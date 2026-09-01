	vexp2pd zmm4, zword [0x84f]
	vexp2pd zmm4, zmm5

%ifdef ERROR
	vexp2pd zmm15, zmm10
	vexp2pd zmm28, zmm21
%endif
