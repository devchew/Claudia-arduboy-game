CXX = g++
CXXFLAGS = -std=c++17 -O2 -Wall -Wno-c++11-narrowing -I.

balance_upgrades: balance_upgrades.cpp ../Sketch/gameLogic.h ../Sketch/State.h
	$(CXX) $(CXXFLAGS) -o $@ $<

run: balance_upgrades
	./balance_upgrades

clean:
	rm -f balance_upgrades

.PHONY: run clean
