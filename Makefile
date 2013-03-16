LDFLAGS+=-lc++
CXXFLAGS+=-std=c++11 -stdlib=libc++ -Isrc -Isrc/Expression -Isrc/Value -Isrc/Statement
CPPFLAGS+=-MD -MP
SOURCES=$(wildcard src/*.cpp src/Expression/*.cpp src/Parser/*.cpp src/Statement/*.cpp src/Value/*.cpp)
OBJECTS=$(SOURCES:%.cpp=%.o)
DEPS=$(SOURCES:%.cpp=%.d)

.PHONY : all
all : build

.PHONY : clean
clean : clean-build clean-deps

.PHONY : clean-build
clean-build :
	@rm -f hap
	@rm -f $(OBJECTS)

.PHONY : clean-deps
clean-deps :
	@rm -f $(DEPS)

.PHONY : build
build : hap

hap : $(OBJECTS)
	$(CXX) -o $@ $(LDFLAGS) $(OBJECTS)

-include $(DEPS)

define DEPENDS_ON_MAKEFILE
$1 : Makefile

endef

$(call DEPENDS_ON_MAKEFILE,hap)
$(foreach OBJECT,$(OBJECTS),$(eval $(call DEPENDS_ON_MAKEFILE,$(OBJECT))))
