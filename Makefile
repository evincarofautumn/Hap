LDFLAGS+=-lc++
CXXFLAGS+=-std=c++11 -stdlib=libc++
CPPFLAGS+=-MD -MP
SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)

.PHONY : all
all : build

.PHONY : clean
clean : clean-build clean-deps

.PHONY : clean-build
clean-build :
	rm -f hap
	rm -f *.o

.PHONY : clean-deps
clean-deps :
	rm -f *.d

.PHONY : build
build : hap

hap : $(OBJECTS)
	$(CXX) -o $@ $(LDFLAGS) $(OBJECTS)

-include $(SOURCES:%.cpp=%.d)

define DEPENDS_ON_MAKEFILE
$1 : Makefile

endef

$(call DEPENDS_ON_MAKEFILE,hap)
$(foreach OBJECT,$(OBJECTS),$(eval $(call DEPENDS_ON_MAKEFILE,$(OBJECT))))
