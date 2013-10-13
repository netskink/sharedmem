
# common/tests/test0 directory makefile


sharedmem: sharedmem.c
	echo "Build sharedmem."
	$(CC) $(DEBUGMODE) $(KERNELVER) $(SBC) $(APPM) $(PPCMODE) $(MYINCS) $@.c $(MYLIBS) -o $@ 



#.PHONY: clean
clean:
	$(RM) -rf *.o *~ core sharedmem objchk*



