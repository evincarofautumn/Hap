LDFLAGS+=-lc++
SOURCE_PATHS=src src/Expression src/Parser src/Statement src/Value
INCFLAGS=$(addprefix -I,$(SOURCE_PATHS))
WARNFLAGS=$(addprefix -W,all error)
CXXFLAGS+=-std=c++11 -stdlib=libc++ $(INCFLAGS) $(WARNFLAGS) -g
CPPFLAGS+=-MD -MP
SOURCES=$(wildcard $(addsuffix /*.cpp,$(SOURCE_PATHS)))
OBJECTS=$(SOURCES:%.cpp=%.o)
DEPS=$(SOURCES:%.cpp=%.d)
TESTER=./test/run.sh
HAP=./hap

.PHONY : all
all : build test

.PHONY : clean
clean : clean-build clean-deps clean-test

.PHONY : clean-build
clean-build :
	@rm -f $(HAP)
	@rm -f $(OBJECTS)

.PHONY : clean-deps
clean-deps :
	@rm -f $(DEPS)

.PHONY : clean-test
clean-test :
	@rm -f test/*.actual

.PHONY : build
build : $(HAP)

$(HAP) : $(OBJECTS)
	$(CXX) -o $@ $(LDFLAGS) $(OBJECTS)

TESTS=$(basename $(notdir $(wildcard test/*.hap)))
define TESTRULE
test-$1 : $(HAP)
	@$(TESTER) $$(realpath $(HAP)) $1
test : test-$1
endef
.PHONY : $(foreach TEST,$(TESTS),test-$(TEST))
$(foreach TEST,$(TESTS),$(eval $(call TESTRULE,$(TEST))))

.PHONY : loc
loc :
	@wc -l $(SOURCES) | sort -n

.PHONY : woc
woc :
	@./tools/woc.pl $(SOURCES)

-include $(DEPS)

define DEPENDS_ON_MAKEFILE
$1 : Makefile

endef

$(call DEPENDS_ON_MAKEFILE,hap)
$(foreach OBJECT,$(OBJECTS),$(eval $(call DEPENDS_ON_MAKEFILE,$(OBJECT))))
