LDFLAGS+=-lc++

LIB_PATHS=lib lib/Expression lib/Parser lib/Statement lib/Value

INCFLAGS=$(addprefix -I,$(LIB_PATHS))
WARNFLAGS=$(addprefix -W,all error)
CXXFLAGS+=-std=c++11 -stdlib=libc++ $(INCFLAGS) $(WARNFLAGS)

CPPFLAGS+=-MD -MP

BIN=./bin
HAP=$(BIN)/hap
HAP_SOURCE_PATHS=hap $(LIB_PATHS)
HAP_SOURCES=$(wildcard $(addsuffix /*.cpp,$(HAP_SOURCE_PATHS)))
HAP_OBJECTS=$(HAP_SOURCES:%.cpp=%.o)
HAP_DEPS=$(HAP_SOURCES:%.cpp=%.d)

UNIT=$(BIN)/hap-unit
UNIT_SOURCE_PATHS=unit $(LIB_PATHS)
UNIT_SOURCES=$(wildcard $(addsuffix /*.cpp,$(UNIT_SOURCE_PATHS)))
UNIT_OBJECTS=$(UNIT_SOURCES:%.cpp=%.o)
UNIT_DEPS=$(UNIT_SOURCES:%.cpp=%.d)

TESTER=./test/run.sh

.PHONY : all
all : build unit-test test

.PHONY : clean
clean : clean-build clean-deps clean-test

.PHONY : clean-build
clean-build :
	@rm -f $(HAP)
	@rm -f $(HAP_OBJECTS)
	@rm -f $(UNIT)
	@rm -f $(UNIT_OBJECTS)

.PHONY : clean-deps
clean-deps :
	@rm -f $(HAP_DEPS)
	@rm -f $(UNIT_DEPS)

.PHONY : clean-test
clean-test :
	@rm -f test/*.actual

.PHONY : build
build : $(HAP)

$(HAP) : $(HAP_OBJECTS)
	mkdir -p $(BIN)
	$(CXX) -o $@ $(LDFLAGS) $(HAP_OBJECTS)

.PHONY : unit-test
unit-test : $(UNIT)
	@$(UNIT)

$(UNIT) : $(UNIT_OBJECTS)
	$(CXX) -o $@ $(LDFLAGS) $(UNIT_OBJECTS)

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
-include $(UNIT_DEPS)

define DEPENDS_ON_MAKEFILE
$1 : Makefile

endef

$(call DEPENDS_ON_MAKEFILE,hap)
$(foreach OBJECT,$(HAP_OBJECTS),$(eval $(call DEPENDS_ON_MAKEFILE,$(OBJECT))))
$(foreach OBJECT,$(UNIT_OBJECTS),$(eval $(call DEPENDS_ON_MAKEFILE,$(OBJECT))))
