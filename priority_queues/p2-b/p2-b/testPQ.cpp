// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

/*
 * Compile this test against your .h files to make sure they compile. We
 * suggest adding to this file or creating your own test cases to test your
 * priority queue implementations more thoroughly. If you do not call a
 * function from here, that template is not instantiated and that function is
 * NOT compiled! So for instance, if you don't add code here to call
 * updatePriorities, that function could later cause compiler errors that you
 * don't even know about.
 *
 * Our makefile will build an executable named testPQ if you type 'make testPQ'
 * or 'make alltests' (without the quotes). This will be a debug executable.
 *
 * Notice that testPairing tests the range-based constructor but main and
 * testPriorityQueue do not. Make sure to test the range-based constructor
 * for other PQ types, and also test the PairingPQ-specific member functions.
 *
 * This is NOT a complete test of your priority queues. You have to add code
 * to do more testing!
 *
 * You do not have to submit this file, but it won't cause problems if you do.
 */

#include <cassert>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <vector>

#include "BinaryPQ.h"
#include "Eecs281PQ.h"
#include "PairingPQ.h"
#include "SortedPQ.h"
#include "UnorderedPQ.h"


// A type for representing priority queue types at runtime
enum class PQType {
    Unordered,
    Sorted,
    Binary,
    Pairing,
};

// These can be pretty-printed :)
std::ostream& operator<<(std::ostream& ost, PQType pqType) {
    switch (pqType) {
    case PQType::Unordered:
        return ost << "Unordered";
    case PQType::Sorted:
        return ost << "Sorted";
    case PQType::Binary:
        return ost << "Binary";
    case PQType::Pairing:
        return ost << "Pairing";
    }

    return ost << "Unknown PQType";
}


// Compares two int const* on the integers they point to
struct IntPtrComp {
    bool operator()(int const* a, int const* b) const { return *a < *b; }
};


// Test the primitive operations on a priority queue: constructor, push, pop, top, size, empty.
template <template <typename...> typename PQ>
void testPrimitiveOperations() {
    std::cout << "Testing the primitive priority queue operations..." << std::endl;

    PQ<int> pq {};
    Eecs281PQ<int>& eecsPQ = pq;

    eecsPQ.push(3);
    eecsPQ.push(4);
    assert(eecsPQ.size() == 2);
    assert(eecsPQ.top() == 4);

    eecsPQ.pop();
    assert(eecsPQ.size() == 1);
    assert(eecsPQ.top() == 3);
    assert(not eecsPQ.empty());

    eecsPQ.pop();
    assert(eecsPQ.size() == 0);
    assert(eecsPQ.empty());

    // TODO: Add more testing here!
    
    eecsPQ.push(3);
    eecsPQ.push(9);
    eecsPQ.push(8);
    eecsPQ.push(2);
    assert(eecsPQ.size() == 4);
    assert(eecsPQ.top() == 9);
    assert(not eecsPQ.empty());
    
    eecsPQ.pop();
    assert(eecsPQ.size() == 3);
    assert(eecsPQ.top() == 8);
    assert(not eecsPQ.empty());
    
    eecsPQ.pop();
    assert(eecsPQ.size() == 2);
    assert(eecsPQ.top() == 3);
    assert(not eecsPQ.empty());
    
    eecsPQ.pop();
    assert(eecsPQ.size() == 1);
    assert(eecsPQ.top() == 2);
    assert(not eecsPQ.empty());
    
    eecsPQ.pop();
    assert(eecsPQ.size() == 0);
    assert(eecsPQ.empty());

    std::cout << "testPrimitiveOperations succeeded!" << std::endl;
}


// Test that the priority queue uses its comparator properly.
// HiddenData can't be compared with operator<, so we use HiddenDataComp{} instead.
template <template <typename...> typename PQ>
void testHiddenData() {
    struct HiddenData {
        int data;
    };

    struct HiddenDataComp {
        bool operator()(HiddenData const& data1, HiddenData const& data2) const {
            return data1.data < data2.data;
        }
    };

    std::cout << "Testing with hidden data..." << std::endl;
    
    // Consider writing this code in the style of testPrimitiveOperations above.

    std::cout << "testHiddenData succeeded!" << std::endl;
}


// Test the last public member function of Eecs281PQ, updatePriorities
template <template <typename...> typename PQ>
void testUpdatePriorities() {
    std::vector<int> data {
        1,
        5,
    };

    PQ<int const*, IntPtrComp> pq {};
    Eecs281PQ<int const*, IntPtrComp>& eecsPQ = pq;

    // NOTE: If you add more data to the vector, don't push the pointers
    //   until AFTER the vector stops changing size! Think about why.
    for (auto& datum : data) {
        eecsPQ.push(&datum);
    }

    // Change some element in data (which is pointed to by an element in pq).
    // This new value should be higher than any other so its address will wind
    //   qt the top adter updatePriorities.
    auto& datum = data[0];
    datum = 10;
    eecsPQ.updatePriorities();
    assert(*eecsPQ.top() == 10);
    assert(eecsPQ.top() == &datum);

    // TODO: Add more testing here as you see fit.
    std::vector<int> data2 {
        3,
        6,
        9,
        4,
        11
    };
    
    PQ<int const*, IntPtrComp> pq2 {};
    Eecs281PQ<int const*, IntPtrComp>& eecsPQ2 = pq2;
    
    for (auto& datum2 : data2) {
        eecsPQ2.push(&datum2);
    }
    
    auto& datum2 = data2[0];
    datum2 = 12;
    eecsPQ2.updatePriorities();
    assert(*eecsPQ2.top() == 12);
    assert(eecsPQ2.top() == &datum2);
    
    std::vector<int> data3 {
        5,
        9,
        3,
        7,
        2
    };
    
    PQ<int const*, IntPtrComp> pq3 {};
    Eecs281PQ<int const*, IntPtrComp>& eecsPQ3 = pq3;
    
    for (auto& datum3 : data3) {
        eecsPQ3.push(&datum3);
    }
    
    auto& datum3 = data3[1];
    datum3 = 1;
    eecsPQ3.updatePriorities();
    assert(*eecsPQ3.top() == 7);
    assert(eecsPQ3.top() != &datum3);
    
}


// Test the pairing heap's range-based constructor, copy constructor,
//   copy-assignment operator, and destructor
// TODO: Test other operations specific to this PQ type.
void testPairing() {
    std::cout << "Testing Pairing Heap separately..." << std::endl;

    {
        std::vector<int> const vec {
            1,
            0,
        };

        std::cout << "Calling constructors" << std::endl;

        // Range-based constructor
        PairingPQ<int> pairing1 { vec.cbegin(), vec.cend() };

        // Copy constructor
        PairingPQ<int> pairing2 { pairing1 };

        // Copy-assignment operator
        PairingPQ<int> pairing3 {};
        pairing3 = pairing2;

        // A reference to a PairingPQ<T> is a reference to an Eecs281PQ<T>.
        // Yay for polymorphism! We can therefore write:
        Eecs281PQ<int>& pq1 = pairing1;
        Eecs281PQ<int>& pq2 = pairing2;
        Eecs281PQ<int>& pq3 = pairing3;

        pq1.push(3);
        pq2.pop();
        assert(pq1.size() == 3);
        assert(not pq1.empty());
        assert(pq1.top() == 3);
        pq2.push(pq3.top());
        assert(pq2.top() == pq3.top());

        std::cout << "Basic tests done." << std::endl;

        // TODO: Add more code to test addNode, updateElt, etc.
        std::cout << "Testing addNode." << std::endl;
        PairingPQ<int> test1;
        test1.addNode(3);
        test1.addNode(7);
        test1.addNode(2);
        test1.addNode(10);
        
        assert(test1.top() == 10);
        assert(test1.size() == 4);
        assert(not test1.empty());
        
        test1.pop();
        test1.pop();
        assert(test1.top() == 3);
        assert(test1.size() == 2);
        assert(not test1.empty());
        
        test1.addNode(1);
        assert(test1.top() == 3);
        assert(test1.size() == 3);
        assert(not test1.empty());
        
        test1.addNode(11);
        test1.addNode(12);
        test1.addNode(10);
        assert(test1.top() == 12);
        assert(test1.size() == 6);
        assert(not test1.empty());
        
        std::cout << "addNode tests done." << endl;
        
        std::cout << "Testing updateElt." << std::endl;
        
        test1.updateElt(test1.addNode(13), 14);
        assert(test1.top() == 14);
        assert(test1.size() == 7);
        assert(not test1.empty());
        
        //child has sibling becomes root
        test1.updateElt(test1.addNode(13), 17);
        assert(test1.top() == 17);
        assert(test1.size() == 8);
        assert(not test1.empty());
        
        test1.pop();
        assert(test1.top() == 14);
        assert(test1.size() == 7);
        assert(not test1.empty());
        
        //child has no sibling and stays child
        PairingPQ<int> test2;
        test2.push(4);
        test2.updateElt(test2.addNode(1), 2);
        assert(test2.top() == 4);
        assert(test2.size() == 2);
        
        //child has sibling and stays child
        test2.updateElt(test2.addNode(1), 3);
        assert(test2.top() == 4);
        assert(test2.size() == 3);

        // That { above creates a scope, and our pairing heaps will fall out of
        //   scope at the matching } below.
        std::cout << "Calling destructors" << std::endl;
    }

    std::cout << "testPairing succeeded!" << std::endl;
}


// Run all tests for a particular PQ type.
template <template <typename...> typename PQ>
void testPriorityQueue() {
    testPrimitiveOperations<PQ>();
    testHiddenData<PQ>();
    testUpdatePriorities<PQ>();
}

// PairingPQ has some extra behavior we need to test in updateElement.
// This template specialization handles that without changing the nice uniform
//   interface of testPriorityQueue.
template <>
void testPriorityQueue<PairingPQ>() {
    testPrimitiveOperations<PairingPQ>();
    testHiddenData<PairingPQ>();
    testUpdatePriorities<PairingPQ>();
    testPairing();
}


int main() {
    std::vector<PQType> const types {
        PQType::Unordered,
        PQType::Sorted,
        PQType::Binary,
        PQType::Pairing,
    };

    std::cout << "PQ tester" << std::endl << std::endl;
    int idx { 0 };
    for (auto const& type : types) {
        std::cout << "  " << idx++ << ") " << type << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Select one: ";
    uint32_t choice {};
    std::cin >> choice;
    PQType const pqType = types.at(choice);

    std::cout << "Testing the " << pqType << " PQ...";

    // TODO: Add more cases to test other priority queue types.
    switch (pqType) {
        case PQType::Unordered:
            testPriorityQueue<UnorderedPQ>();
            break;
        case PQType::Sorted:
            testPriorityQueue<SortedPQ>();
            break;
        case PQType::Binary:
            testPriorityQueue<BinaryPQ>();
            break;
        case PQType::Pairing:
            testPriorityQueue<PairingPQ>();
            break;
        default:
        std::cout << "Unrecognized PQ type " << pqType << " in main." << std::endl
                  << "Perhaps you forgot to add tests for all four PQ types." << std::endl;
        return 1;
    }

    std::cout << "All tests succeeded!" << std::endl;

    return 0;
}
