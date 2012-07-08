
INCLUDES = -I..
VPATH = ..

OBJDIR = .

SOURCES = entity.cpp abstractproperty.cpp entexample.cpp

# Define the object files. These will live in a directory structure in the currently defind OBJDIR
OBJECTS += $(patsubst %.cpp,$(OBJDIR)/%.obj, $(filter %.cpp, $(SOURCES)))

all : entexample

entexample: $(OBJECTS)
	g++ -o $@ $(INCLUDES) $(OBJECTS)

%.obj : %.cpp
	g++ -c $< -o $@ $(INCLUDES)

	
clean:
	rm $(OBJECTS)
