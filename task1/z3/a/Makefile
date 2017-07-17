O=2
CC=gcc -Wall -O$(O)
.PHONY: clean

#compilation with main
test_static: main.c static.a
	$(CC) $< -L. -lstatic -o $@

#libraries
static.a: static_contact.a static_list.a static_bst.a
	ar crs $@ $^
shared.so: shared_contact.so shared_list.so shared_bst.so
	$(CC) -fPIC -shared $^ -o $@ 


#Contact
static_contact.a: contact.c contact.h
	ar crs $@ $^
shared_contact.so: contact.c contact.h
	$(CC) -fPIC -shared $^ -o $@

#DLlist
static_list.a: list.c list.h
	ar crs $@ $^
shared_list.so: list.c list.h
	$(CC) -fPIC -shared $^ -o $@

#BST
static_bst.a: bst.c bst.h
	ar crs $@ $^
shared_bst.so: bst.c bst.h
	$(CC) -fPIC -shared $^ -o $@


clean:
	rm -f *.so *.a

clean_rest:
	rm static_contact.a shared_contact.so shared_bst.so static_bst.a shared_list.so static_list.a

