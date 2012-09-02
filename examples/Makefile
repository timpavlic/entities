
INCLUDES = -I.. -I../.. -I../../sqlite3

VPATH = ..
VPATH += ../../sqlite3
VPATH += ../factories

OBJDIR = .

SOURCES = entity.cpp abstractproperty.cpp entception.cpp

# Define the object files. These will live in a directory structure in the currently defind OBJDIR
COMMON_OBJECTS += $(patsubst %.cpp,$(OBJDIR)/%.obj, $(filter %.cpp, $(SOURCES)))

printtargets :
	@echo The available example targets are:
	@echo  * entexample
	@echo  * sqlite3example: We expect to find an sqlite3 folder in the same
	@echo                    folder you have the entities repo cloned in.

entexample: $(COMMON_OBJECTS) entexample.obj
	g++ -o $@ $(INCLUDES) $(COMMON_OBJECTS)
	
sqlite3example: $(COMMON_OBJECTS) sqlite3example.obj sqlite3.obj sqlite3entityfactory.obj sqlite3persistenceapi.obj
	g++ -o $@ $(INCLUDES) $^

%.obj : %.cpp
	g++ -c $< -o $@ $(INCLUDES)

%.obj : %.c
	gcc -c $< -o $@ $(INCLUDES)
	
clean:
	rm $(COMMON_OBJECTS)

printinfo:
	@echo COMMON_OBJECTS  = $(COMMON_OBJECTS)
	@echo SOURCES         = $(SOURCES)
	