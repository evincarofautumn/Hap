LDFLAGS+=-lc++
SOURCE_PATHS=src src/Expression src/Parser src/Statement src/Value
INCFLAGS=$(addprefix -I,$(SOURCE_PATHS))
WARNFLAGS=$(addprefix -W,all error)
CXXFLAGS+=-std=c++11 -stdlib=libc++ $(INCFLAGS) $(WARNFLAGS)
CPPFLAGS+=-MD -MP
SOURCES=$(wildcard $(addsuffix /*.cpp,$(SOURCE_PATHS)))
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
