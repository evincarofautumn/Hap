LDFLAGS+=-lc++

LIB_PATHS=lib lib/Expression lib/Parser lib/Statement lib/Value
HAP_SOURCE_PATHS=hap $(LIB_PATHS)

INCFLAGS=$(addprefix -I,$(LIB_PATHS))
WARNFLAGS=$(addprefix -W,all error)
CXXFLAGS+=-std=c++11 -stdlib=libc++ $(INCFLAGS) $(WARNFLAGS)

CPPFLAGS+=-MD -MP

HAP=./bin/hap
HAP_SOURCES=$(wildcard $(addsuffix /*.cpp,$(HAP_SOURCE_PATHS)))
HAP_OBJECTS=$(HAP_SOURCES:%.cpp=%.o)
HAP_DEPS=$(HAP_SOURCES:%.cpp=%.d)

TESTER=./test/run.sh

.PHONY : all
all : build test

.PHONY : clean
clean : clean-build clean-deps clean-test

.PHONY : clean-build
clean-build :
	@rm -f $(HAP)
	@rm -f $(HAP_OBJECTS)

.PHONY : clean-deps
clean-deps :
	@rm -f $(HAP_DEPS)

.PHONY : clean-test
clean-test :
	@rm -f test/*.actual

.PHONY : build
build : $(HAP)

$(HAP) : $(HAP_OBJECTS)
	$(CXX) -o $@ $(LDFLAGS) $(HAP_OBJECTS)

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
	@wc -l $(HAP_SOURCES) | sort -n

.PHONY : woc
woc :
	@./tools/woc.pl $(HAP_SOURCES)

-include $(HAP_DEPS)

define DEPENDS_ON_MAKEFILE
$1 : Makefile

endef

$(call DEPENDS_ON_MAKEFILE,hap)
$(foreach OBJECT,$(HAP_OBJECTS),$(eval $(call DEPENDS_ON_MAKEFILE,$(OBJECT))))
