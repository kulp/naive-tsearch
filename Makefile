all: check

# test_tsearch exercises the code, but does not yet check it for correctness
check: test_tsearch
	$(realpath $<)

clean:
	$(RM) *.o test_tsearch

