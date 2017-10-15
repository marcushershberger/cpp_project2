// Provided by:   Marcus Hershberger
// Lab:           180
// Course number: 221
// Besides the code that was provided for the assignment, all code is my own.
// FILE: sequence.h
// CLASS PROVIDED: sequence (part of the namespace main_savitch_4)
//
// VALUE SEMANTICS for the sequence class:
//    Assignments and the copy constructor may be used with sequence objects.
//
// DYNAMIC MEMORY USAGE by the List
//   If there is insufficient dynamic memory, then the following functions
//   throw a BAD_ALLOC exception: The constructors, insert, attach. 


#ifndef MAIN_SAVITCH_SEQUENCE_H
#define MAIN_SAVITCH_SEQUENCE_H
#include <cstdlib>  // Provides size_t
#include <algorithm>
#include <cassert>

namespace main_savitch_4
{
  class sequence
  {
  public:
    // TYPEDEFS and MEMBER CONSTANTS for the sequence class:

    // sequence::value_type is the data type of the items in the sequence.
    // It may be any of the C++ built-in types (int, char, etc.), or a
    // class with a default constructor, an assignment operator, and a
    // copy constructor.
    typedef double value_type;

    // sequence::size_type is the data type of any variable that keeps
    // track of how many items are in a sequence.
    typedef std::size_t size_type;

    // sequence::DEFAULT_CAPACITY is the initial capacity of a sequence
    // that is created by the default constructor.
    static const size_type DEFAULT_CAPACITY = 30;

    // CONSTRUCTORS and DESTRUCTOR

    // CONSTRUCTOR
    // Postcondition: The sequence has been initialized as an empty sequence.
    // The insert/attach functions will work efficiently (without allocating
    // new memory) until this capacity is reached.
    sequence(size_type initial_capacity = DEFAULT_CAPACITY);

    // COPY CONSTRUCTOR
    // Postcondition: The sequence has been initialized to the sequence
    //   passed in.
    // The insert/attach functions will work efficiently (without allocating
    // new memory) until the source's capacity is reached.
    sequence(const sequence& source);

    // DESTRUCTOR
    // Postcondition: The memory allocated for the object has been released.
    ~sequence( );

    // MODIFICATION MEMBER FUNCTIONS

    // Postcondition: The sequence's current capacity is changed to the 
    //   new_capacity (but not less that the number of items already on the
    //   list). The insert/attach functions will work efficiently (without
    //   allocating new memory) until this new capacity is reached.
    void resize(size_type new_capacity);

    // Postcondition: The first item on the sequence becomes the current item
    //   (but if the sequence is empty, then there is no current item).
    void start( );

    // Precondition: is_item returns true.
    // Postcondition: If the current item was already the last item in the
    //   sequence, then there is no longer any current item. Otherwise, the new
    //   current item is the item immediately after the original current item.
    void advance( );

    // Postcondition: A new copy of entry has been inserted in the sequence
    //   before the current item. If there was no current item, then the new
    //   entry has been inserted at the front of the sequence. In either case,
    //   the newly inserted item is now the current item of the sequence.
    void insert(const value_type& entry);

    // Postcondition: A new copy of entry has been inserted in the sequence
    //   after the current item. If there was no current item, then the new
    //   entry has been attached to the end of the sequence. In either case,
    //   the newly inserted item is now the current item of the sequence.
    void attach(const value_type& entry);

    // Precondition: is_item returns true.
    // Postcondition: The current item has been removed from the sequence,
    //   and the item after this (if there is one) is now the new current item.
    void remove_current( );

    // Postcondition: The current object is equivalent to the object passed
    //   in.
    void operator =(const sequence& source);

    // CONSTANT MEMBER FUNCTIONS

    // Postcondition: The return value is the number of items in the sequence.
    size_type size( ) const;

    // Postcondition: A true return value indicates that there is a valid
    //   "current" item that may be retrieved by activating the current
    //   member function (listed below). A false return value indicates that
    //   there is no valid current item.
    bool is_item( ) const;

    //  Precondition: is_item( ) returns true.
    //  Postcondition: The item returned is the current item in the sequence.
    value_type current( ) const;

    //  Postcondition: The return value is true if both sequences contain no
    //    data items. Also, if the length is greater than zero, then the
    //    sequences must contain the same number of items and the items at the
    //    same location must be equal.
    bool operator ==(const sequence& other);

    //  Postcondition: The return value is false if both sequences contain no
    //    data items. Also, if the length is greater than zero, then the
    //    sequences must not contain the same number of items or the items at
    //    the same location must not be equal.
    bool operator !=(const sequence& other);

  private:
    // HELPER (PRIVATE) FUNCTIONS

    // Precondition: The capacity is (at least one) bigger than the number of
    //   items stored.
    // Postcondition: All data items from index to the number of items stored
    //   will be moved down one space.
    void move_down(size_type index);

    // Precondition: There is at least one item stored.
    // Postcondition: All data items from index to the number of items stored
    //   will be moved up one space.
    void move_up(size_type index);

    // DATA MEMBERS

    //  The array of data items.
    value_type* data_;

    // The count of data items.
    size_type used_;

    // The position of the internal iterator.
    size_type current_index_;

    // The maximum number of items that can be held.
    size_type capacity_;
  };
}

#endif
