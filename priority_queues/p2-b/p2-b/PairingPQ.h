// Project identifier: 43DE0E0C4C76BFAA6D8C2F5AEAE0518A9C42CF4E

#ifndef PAIRINGPQ_H
#define PAIRINGPQ_H

#include "Eecs281PQ.h"
#include <deque>
#include <utility>

// A specialized version of the priority queue ADT implemented as a pairing
// heap.
template<typename TYPE, typename COMP_FUNCTOR = std::less<TYPE>>
class PairingPQ : public Eecs281PQ<TYPE, COMP_FUNCTOR> {
    // This is a way to refer to the base class object.
    using BaseClass = Eecs281PQ<TYPE, COMP_FUNCTOR>;

public:
    // Each node within the pairing heap
    class Node {
        public:
            // TODO: After you add add one extra pointer (see below), be sure
            //       to initialize it here.
            explicit Node(const TYPE &val)
            : elt{ val }, child{ nullptr }, sibling{ nullptr }, previous{ nullptr }
            {}

            // Description: Allows access to the element at that Node's
            //              position. There are two versions, getElt() and a
            //              dereference operator, use whichever one seems
            //              more natural to you.
            // Runtime: O(1) - this has been provided for you.
            const TYPE &getElt() const { return elt; }
            const TYPE &operator*() const { return elt; }

            // The following line allows you to access any private data
            // members of this Node class from within the PairingPQ class.
            // (ie: myNode.elt is a legal statement in PairingPQ's add_node()
            // function).
            friend PairingPQ;

        private:
            TYPE elt;
            Node *child;
            Node *sibling;
            Node *previous;
    }; // Node


    // Description: Construct an empty pairing heap with an optional
    //              comparison functor.
    // Runtime: O(1)
    explicit PairingPQ(COMP_FUNCTOR comp = COMP_FUNCTOR()) :
        BaseClass{ comp }, root{ nullptr }, num_nodes{ 0 } {
    } // PairingPQ()


    // Description: Construct a pairing heap out of an iterator range with an
    //              optional comparison functor.
    // Runtime: O(n) where n is number of elements in range.
    template<typename InputIterator>
    PairingPQ(InputIterator start, InputIterator end, COMP_FUNCTOR comp = COMP_FUNCTOR()) :
    BaseClass{ comp } , root{ nullptr }, num_nodes{ 0 } {
        // TODO: Implement this function.
            while (start != end) {
                push(*start);
                start++;
            }
    } // PairingPQ()


    // Description: Copy constructor.
    // Runtime: O(n)
    PairingPQ(const PairingPQ &other) :
        BaseClass{ other.compare }, root{ nullptr }, num_nodes{ 0 } {
            if (other.root == nullptr) {
                return;
            }
            std::deque<Node*> temp;
            temp.push_back(other.root);
            while(!temp.empty()) {
                Node *p = temp.front();
                temp.pop_front();
                if (p->child != nullptr) {
                    temp.push_back(p->child);
                }
                if (p->sibling != nullptr)  {
                    temp.push_back(p->sibling);
                }
                push(p->elt);
            }
    } // PairingPQ()


    // Description: Copy assignment operator.
    // Runtime: O(n)
    PairingPQ &operator=(const PairingPQ &rhs) {
        // TODO: Implement this function.
        PairingPQ temp(rhs);
        
        std::swap(num_nodes, temp.num_nodes);
        std::swap(root, temp.root);
        
        return *this;
    } // operator=()


    // Description: Destructor
    // Runtime: O(n)
    ~PairingPQ() {
        // TODO: Implement this function.
        if (root == nullptr) {
            return;
        }
        std::deque<Node*> temp;
        temp.push_back(root);
        while(!temp.empty()) {
            Node *p = temp.front();
            temp.pop_front();
            if (p->child != nullptr) {
                temp.push_back(p->child);
            }
            if (p->sibling != nullptr)  {
                temp.push_back(p->sibling);
            }
            delete p;
        }
    } // ~PairingPQ()


    // Description: Assumes that all elements inside the pairing heap are out
    //              of order and 'rebuilds' the pairing heap by fixing the
    //              pairing heap invariant. You CANNOT delete 'old' nodes
    //              and create new ones!
    // Runtime: O(n)
    virtual void updatePriorities() {
        // TODO: Implement this function.
        if (root == nullptr) {
            return;
        }
        std::deque<Node*> temp;
        temp.push_back(root);
        root = nullptr;
        while(!temp.empty()) {
            Node *p = temp.front();
            temp.pop_front();
            if (p->child != nullptr) {
                temp.push_back(p->child);
            }
            if (p->sibling != nullptr)  {
                temp.push_back(p->sibling);
            }
            p->child = nullptr;
            p->sibling = nullptr;
            p->previous = nullptr;
            if (root == nullptr) {
                root = p;
            }
            else {
                root = meld(root, p);
            }
        }
    } // updatePriorities()


    // Description: Add a new element to the pairing heap. This is already
    //              done. You should implement push functionality entirely in
    //              the addNode() function, and this function calls
    //              addNode().
    // Runtime: O(1)
    virtual void push(const TYPE &val) {
        addNode(val);
    } // push()


    // Description: Remove the most extreme (defined by 'compare') element
    //              from the pairing heap.
    // Note: We will not run tests on your code that would require it to pop
    //       an element when the pairing heap is empty. Though you are
    //       welcome to if you are familiar with them, you do not need to use
    //       exceptions in this project.
    // Runtime: Amortized O(log(n))
    virtual void pop() {
        // TODO: Implement this function.
        Node *p = root->child;
        delete root;
        if (p == nullptr) {
            root = nullptr;
        }
        else {
            std::deque<Node*> temp;
            while (p != nullptr) {
                temp.push_back(p);
                p = p->sibling;
            }
            while (temp.size() > 1) {
                Node *pa = temp.front();
                pa->previous = nullptr;;
                pa->sibling = nullptr;
                temp.pop_front();
                Node *pb = temp.front();
                pb->previous = nullptr;
                pb->sibling = nullptr;
                temp.pop_front();
                temp.push_back(meld(pa, pb));
            }
            temp[0]->previous = nullptr;
            root = temp[0];
        }
        num_nodes--;
    } // pop()


    // Description: Return the most extreme (defined by 'compare') element of
    //              the pairing heap. This should be a reference for speed.
    //              It MUST be const because we cannot allow it to be
    //              modified, as that might make it no longer be the most
    //              extreme element.
    // Runtime: O(1)
    virtual const TYPE &top() const {
        // TODO: Implement this function
        return root->elt;
    } // top()


    // Description: Get the number of elements in the pairing heap.
    // Runtime: O(1)
    virtual std::size_t size() const {
        return num_nodes;
    } // size()

    // Description: Return true if the pairing heap is empty.
    // Runtime: O(1)
    virtual bool empty() const {
        return num_nodes == 0;
    } // empty()


    // Description: Updates the priority of an element already in the pairing
    //              heap by replacing the element refered to by the Node with
    //              new_value. Must maintain pairing heap invariants.
    //
    // PRECONDITION: The new priority, given by 'new_value' must be more
    //               extreme (as defined by comp) than the old priority.
    //
    // Runtime: As discussed in reading material.
    void updateElt(Node* node, const TYPE &new_value) {
        node->elt = new_value;
        if (node == root) {
            return;
        }
        else if (node == node->previous->child) {
            if (node->sibling != nullptr) {
                node->previous->child = node->sibling;
                node->sibling->previous = node->previous;
            }
            else {
                node->previous->child = nullptr;
            }
        }
        else {
            if (node->sibling != nullptr) {
                node->previous->sibling = node->sibling;
                node->sibling->previous = node->previous;
            }
            else {
                node->previous->sibling = nullptr;
            }
        }
        node->sibling = nullptr;
        node->previous = nullptr;
        meld(root, node);
    } // updateElt()


    // Description: Add a new element to the pairing heap. Returns a Node*
    //              corresponding to the newly added element.
    // NOTE: Whenever you create a node, and thus return a Node *, you must
    //       be sure to never move or copy/delete that node in the future,
    //       until it is eliminated by the user calling pop(). Remember this
    //       when you implement updateElt() and updatePriorities().
    // Runtime: O(1)
    Node* addNode(const TYPE &val) {
        // TODO: Implement this function
        Node *p = new Node(val);
        if (root == nullptr) {
            root = p;
        }
        else {
            meld(root, p);
        }
        num_nodes++;
        return p;
    } // addNode()


private:
    Node *root;
    size_t num_nodes;
    
    Node* meld(Node *pa, Node *pb) {
        if (this->compare(pa->elt, pb->elt)) {
            if (pb->child == nullptr) {
                pb->child = pa;
                pa->previous = pb;
            }
            else {
                pa->sibling = pb->child;
                pa->previous = pb;
                pb->child->previous = pa;
                pb->child = pa;
            }
            if (root == pa) {
                root = pb;
            }
            return pb;
        }
        else {
            if (pa->child == nullptr) {
                pa->child = pb;
                pb->previous = pa;
            }
            else {
                pb->sibling = pa->child;
                pb->previous = pa;
                pa->child->previous = pb;
                pa->child = pb;
            }
            if (root == pb) {
                root = pa;
            }
            return pa;
        }
    }
    // NOTE: For member variables, you are only allowed to add a "root
    //       pointer" and a "count" of the number of nodes. Anything else
    //       (such as a deque) should be declared inside of member functions
    //       as needed.
};


#endif // PAIRINGPQ_H
