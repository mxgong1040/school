CXXFLAGS += -std=c++11
GTKFLAGS =`/usr/bin/pkg-config gtkmm-3.0 --cflags --libs`

ex: main.o battle.o
	$(CXX) $(CXXFLAGS) -o battle main.o battle.o $(GTKFLAGS)
main.o: main.cpp battle.h
	$(CXX) $(CXXFLAGS) -c main.cpp $(GTKFLAGS)
battle.o: battle.h battle.cpp
	$(CXX) $(CXXFLAGS) -c battle.cpp $(GTKFLAGS)
