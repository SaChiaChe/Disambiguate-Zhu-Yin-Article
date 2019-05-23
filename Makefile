SRIPATH ?= /home/student/05/b05902030/DSP/HW3/srilm-1.5.10
MACHINE_TYPE ?= i686-m64

CXX = g++
CXXFLAGS = -O3 -I$(SRIPATH)/include -w --std=c++11
vpath lib%.a $(SRIPATH)/lib/$(MACHINE_TYPE)

TARGET = mydisambig
SRC = mydisambig.cpp
OBJ = $(SRC:.cpp=.o)
TO = ZhuYin-Big5.map
FROM = Big5-ZhuYin.map
LM = bigram.lm

.PHONY: all clean run runSRILM map

all: $(TARGET)

$(TARGET): $(OBJ) -loolm -ldstruct -lmisc
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

run:
	@for i in $(shell seq 1 10) ; do \
	    echo "./mydisambig $(LM) $(TO) testdata/$$i.txt > result2/$$i.txt"; \
	    ./mydisambig $(LM) $(TO) testdata/$$i.txt > result2/$$i.txt; \
	done;

runSRILM:
	@for i in $(shell seq 1 10) ; do \
	    echo "./disambig -text testdata/$$i.txt -map $(TO) -lm $(LM) -order 2 > result1/$$i.txt"; \
	    ./disambig -text testdata/$$i.txt -map $(TO) -lm $(LM) -order 2 > result1/$$i.txt; \
	done;

map:
	@echo "python3 mapping.py $(FROM) $(TO)"
	@python3 mapping.py $(FROM) $(TO)

clean:
	$(RM) $(OBJ) $(TARGET)