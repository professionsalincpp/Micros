#ifndef __PAIR_H
#define __PAIR_H

template <typename T1, typename T2>
class OPair {
public:
    T1 first;
    T2 second;
    OPair(T1 first, T2 second) : first(first), second(second) {}
    OPair() : first(), second() {}
    OPair(T1 first) : first(first), second() {}

    T1& getFirst() { return first; }
    T2& getSecond() { return second; }

    T1 getFirst() const { return first; }
    T2 getSecond() const { return second; }

    void setFirst(T1 first) { this->first = first; }
    void setSecond(T2 second) { this->second = second; }
};


#endif