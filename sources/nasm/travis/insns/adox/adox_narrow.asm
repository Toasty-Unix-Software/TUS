	adox ebx, dword [0xd6b]
	adox ebp, esi
	adox eax, edx, dword [0x1df]
	adox edx, dword [0x1df]
	adox eax, ebx, dword [0xe7e]

%ifdef ERROR
	adox rbx, qword [0x467]
	adox rbp, rax
	adox rdx, rcx, rax
	adox rcx, rax
	adox rbx, rdx, qword [0x1f1]
	adox r10d, r8d
	adox r15, r14
	adox r14d, r11d, r10d
	adox r8, r9, r10
	adox r21d, r17d
	adox r23, r21
	adox r18d, r25d, r21d
	adox r17, r22, r17
	adox rcx, qword [eax+1]
	adox rdx, qword [eax+64]
	adox rsi, rax, qword [eax+1]
	adox rdi, rsi, qword [eax+64]
	adox rbp, [0x542]
	adox rax, rax, [0x92d]
%endif
