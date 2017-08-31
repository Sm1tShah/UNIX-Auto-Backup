#Smit Shah sss160030@utdallas.edu 3376.001
CXX = g++
CXXFLAGS = 
# -L /home/010/s/ss/sss160030/unix/lib
#-lrudeconfig
# -O -L(linker path to libraries)
CPPFLAGS = -Wall -I /home/010/s/ss/sss160030/unix/include
LDFLAGS =
#what will go to the linker
LDLIBS =
#-lcryptopp


PROJECTNAME = Prog5
EXECFILE = Prog5	
OBJS = Prog5.o time.o
INPUT =
PATH1 = /home/010/s/ss/sss160030/unix/lib/librudeconfig.a
HFILE = Prog5.h

%.o:%.cc
	$(CXX) $(CPPFLAGS) -M -MF $*.P $<
	$(CXX) -c $(CXXFLAGS) $(CPPFLAGS) $< -o $@

all: $(EXECFILE) $(HFILE)

$(EXECFILE): $(INPUT) $(OBJS)
	$(CXX) -o $@ $(OBJS) $(PATH1)

clean:
	rm -f $(OBJS) *.P *~ $(EXECFILE)

backup:
	@mkdir -p ~/backups; chmod 700 ~/backups
	@$(eval CURDIRNAME := $(shell basename `pwd`))
	@$(eval MKBKUPNAME := ~/backups/$(PROJECTNAME)-$(shell date +'%Y.%m.%d-%H:%M:%S').tar.gz)
	@echo
	@echo Writing Backup file to: $(MKBKUPNAME)
	@echo
	@-tar zcf $(MKBKUPNAME) ../$(CURDIRNAME) 2> /dev/null
	@chmod 600 $(MKBKUPNAME)
	@echo
	@echo Done!

-include $(OBJS:%.o=%.P)