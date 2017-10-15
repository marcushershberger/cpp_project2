// FILE: sequence_exam.cpp
// Written by: Michael Main (main@colorado.edu) - Oct 29, 1997
// Non-interactive test program for the sequence class using a dynamic array,
// with improved test for heap leaks.
//
// DESCRIPTION:
// Each function of this program tests part of the sequence class, returning
// some number of points to indicate how much of the test was passed.
// A description and result of each test is printed to std::cout.
// Maximum number of points awarded by this program is determined by the
// constants POINTS[1], POINTS[2]...

#include <iostream>    // Provides std::cout.
#include <cstring>     // Provides memcpy.
#include <cstdlib>     // Provides size_t.
#include "sequence.h"  // Provides the Sequence class with double items.
using namespace main_savitch_4;

// Descriptions and points for each of the tests:
const size_t MANY_TESTS = 7;
const int POINTS[MANY_TESTS+1] = {
    70,  // Total points for all tests.
    13,  // Test 1 points
    13,  // Test 2 points
    12,  // Test 3 points
     7,  // Test 4 points
     7,  // Test 5 points
     7,  // Test 6 points
    11,  // Test 7 points
    //10,  // Test 8 points
};
const char DESCRIPTION[MANY_TESTS+1][256] = {
    "tests for sequence class with a dynamic array",
    "Testing insert, attach, and the constant member functions",
    "Testing situations where the cursor goes off the sequence",
    "Testing remove_current",
    "Testing the resize member function",
    "Testing the copy constructor",
    "Testing the assignment operator",
    "Testing insert/attach when current DEFAULT_CAPACITY exceeded"//,
    //"Testing the operator overloads"
};


// **************************************************************************
// bool test_basic(const sequence& test, size_t s, bool has_cursor)
//   Postcondition: A return value of true indicates:
//     a. test.size() is s, and
//     b. test.is_item() is has_cursor.
//   Otherwise the return value is false.
//   In either case, a description of the test result is printed to std::cout.
// **************************************************************************
bool test_basic(const sequence& test, size_t s, bool has_cursor)
{
    bool answer;

    std::cout << "Testing that size() returns " << s << " ... ";
    std::cout.flush( );
    answer = (test.size( ) == s);
    std::cout << (answer ? "Passed." : "Failed.") << std::endl;
    
    if (answer)
    {
        std::cout << "Testing that is_item() returns ";
        std::cout << (has_cursor ? "true" : "false") << " ... ";
        std::cout.flush( );
        answer = (test.is_item( ) == has_cursor);
        std::cout << (answer ? "Passed." : "Failed.") << std::endl;
    }

    return answer;
}


// **************************************************************************
// bool test_items(sequence& test, size_t s, size_t i, double items[])
//   The function determines if the test sequence has the correct items
//   Precondition: The size of the items array is at least s.
//   Postcondition: A return value of true indicates that test.current()
//   is equal to items[i], and after test.advance() the result of
//   test.current() is items[i+1], and so on through items[s-1].
//   At this point, one more advance takes the cursor off the sequence.
//   If any of this fails, the return value is false.
//   NOTE: The test sequence has been changed by advancing its cursor.
// **************************************************************************
bool test_items(sequence& test, size_t s, size_t i, double items[])
{
    bool answer = true;
    
    std::cout << "The cursor should be at item [" << i << "]" << " of the"
	      << " sequence" << std::endl
	      << "(counting the first item as [0]). I will advance the "
	      << "cursor" << std::endl
	      << "to the end of the sequence, checking that each item is "
	      << "correct..." << std::endl;
    while ((i < s) && test.is_item( ) && (test.current( ) == items[i]))
    {
        i++;
        test.advance( );
    }

    if ((i != s) && !test.is_item( ))
    {   // The test.is_item( ) function returns false too soon.
        std::cout << "\n    Cursor fell off the sequence too soon." << std::endl;
        answer = false;
    }
    else if (i != s)
    {   // The test.current( ) function returned a wrong value.
        std::cout << "\n    The item [" << i << "] should be " << items[i] << ",\n";
        std::cout << "    but it was " << test.current( ) << " instead.\n";
        //test.start();
        for (int i  = 0; i < (int)(test.size()); i++)
        {
            std::cout << "]]]]]]]]]]]]   " << items[i] << std::endl;
            //test.advance();
        }
        
        answer = false;
    }
    else if (test.is_item( ))
    {   // The test.is_item( ) function returns true after moving off the sequence.
        std::cout << "\n    The cursor was moved off the sequence,";
        std::cout << " but is_item still returns true." << std::endl;
        answer = false;
    }

    std::cout << (answer ? "Passed." : "Failed.") << std::endl;
    return answer;
}


// **************************************************************************
// bool correct(sequence& test, size_t s, size_t cursor_spot, double items[])
//   This function determines if the sequence (test) is "correct" according to
//   these requirements:
//   a. it has exactly s items.
//   b. the items (starting at the front) are equal to
//      items[0] ... items[s-1]
//   c. if cursor_spot < s, then test's cursor must be at
//      the location given by cursor_spot.
//   d. if cursor_spot >= s, then test must not have a cursor.
// NOTE: The function also moves the cursor off the sequence.
// **************************************************************************
bool correct(sequence& test, size_t size, size_t cursor_spot, double items[])
{
    bool has_cursor = (cursor_spot < size); 

    // Check the sequence's size and whether it has a cursor.
    if (!test_basic(test, size, has_cursor))
    {
        std::cout << "Basic test of size() or is_item() failed." << std::endl << std::endl;
        return false;
    }

    // If there is a cursor, check the items from cursor to end of the sequence.
    if (has_cursor && !test_items(test, size, cursor_spot, items))
    {
        std::cout << "Test of the sequence's items failed." << std::endl << std::endl;
        return false;
    }

    // Restart the cursor at the front of the sequence and test items again.
    std::cout << "I'll call start() and look at the items one more time..." << std::endl;
    test.start( );
    if (has_cursor && !test_items(test, size, 0, items))
    {
        std::cout << "Test of the sequence's items failed." << std::endl << std::endl;
        return false;
    }

    // If the code reaches here, then all tests have been passed.
    std::cout << "All tests passed for this sequence." << std::endl << std::endl;
    return true;
}


// **************************************************************************
// int test1( )
//   Performs some basic tests of insert, attach, and the constant member
//   functions. Returns POINTS[1] if the tests are passed. Otherwise returns 0.
// **************************************************************************
int test1( )
{
    sequence empty;                            // An empty sequence
    sequence test;                             // A sequence to add items to
    double items1[4] = { 5, 10, 20, 30 };  // These 4 items are put in a sequence
    double items2[4] = { 10, 15, 20, 30 }; // These are put in another sequence

    // Test that the empty sequence is really empty
    std::cout << "Starting with an empty sequence." << std::endl;
    if (!correct(empty, 0, 0, items1)) return 0;

    // Test the attach function to add something to an empty sequence
    std::cout << "I am now using attach to put 10 into an empty sequence." << std::endl;
    test.attach(10);
    if (!correct(test, 1, 0, items2)) return 0;
    
    // Test the insert function to add something to an empty sequence
    std::cout << "I am now using insert to put 10 into an empty sequence." << std::endl;
    test = empty;
    test.insert(10);
    if (!correct(test, 1, 0, items2)) return 0;
    
    // Test the insert function to add an item at the front of a sequence
    std::cout << "I am now using attach to put 10,20,30 in an empty sequence.\n";
    std::cout << "Then I move the cursor to the start and insert 5." << std::endl;
    test = empty;
    test.attach(10);
    test.attach(20);
    test.attach(30);
    test.start( );
    test.insert(5);
    if (!correct(test, 4, 0, items1)) return 0;
    
    // Test the insert function to add an item in the middle of a sequence
    std::cout << "I am now using attach to put 10,20,30 in an empty sequence.\n";
    std::cout << "Then I move the cursor to the start, advance once, ";
    std::cout << "and insert 15." << std::endl;
    test = empty;
    test.attach(10);
    test.attach(20);
    test.attach(30);
    test.start( );
    test.advance( );
    test.insert(15);
    if (!correct(test, 4, 1, items2)) return 0;

    // Test the attach function to add an item in the middle of a sequence
    std::cout << "I am now using attach to put 10,20,30 in an empty sequence.\n";
    std::cout << "Then I move the cursor to the start and attach 15 ";
    std::cout << "after the 10." << std::endl;
    test = empty;
    test.attach(10);
    test.attach(20);
    test.attach(30);
    test.start( );
    test.attach(15);
    if (!correct(test, 4, 1, items2)) return 0;

    // All tests have been passed
    std::cout << "All tests of this first function have been passed." << std::endl;
    return POINTS[1];
}


// **************************************************************************
// int test2( )
//   Performs a test to ensure that the cursor can correctly be run off the end
//   of the sequence. Also tests that attach/insert work correctly when there is
//   no cursor. Returns POINTS[2] if the tests are passed. Otherwise returns 0.
// **************************************************************************
int test2( )
{
    sequence test;
    size_t i;

    // Put three items in the sequence
    std::cout << "Using attach to put 20 and 30 in the sequence, and then calling\n";
    std::cout << "advance, so that is_item should return false ... ";
    std::cout.flush( );
    test.attach(20);
    test.attach(30);
    test.advance( );
    if (test.is_item( ))
    {
        std::cout << "failed." << std::endl;
        return 0;
    }
    std::cout << "passed." << std::endl;

    // Insert 10 at the front and run the cursor off the end again
    std::cout << "Inserting 10, which should go at the sequence's front." << std::endl;
    std::cout << "Then calling advance three times to run cursor off the sequence ...";
    std::cout.flush( );
    test.insert(10);
    test.advance( ); // advance to the 20
    test.advance( ); // advance to the 30
    test.advance( ); // advance right off the sequence
    if (test.is_item( ))
    {
        std::cout << " failed." << std::endl;
        return false;
    }
    std::cout << " passed." << std::endl;
    
    // Attach more items until the sequence becomes full.
    // Note that the first attach should attach to the end of the sequence.
    std::cout << "Calling attach to put the numbers 40, 50, 60 ...";
    std::cout << test.DEFAULT_CAPACITY*10 << " at the sequence's end." << std::endl;
    for (i = 4; i <= test.DEFAULT_CAPACITY; i++)
        test.attach(i*10);

    // Test that the sequence is correctly filled.
    std::cout << "Now I will test that the sequence has 10, 20, 30, ...";
    std::cout << test.DEFAULT_CAPACITY*10 << "." << std::endl;
    test.start( );
    for (i = 1; i <= test.DEFAULT_CAPACITY; i++)
    {
        if ((!test.is_item( )) || test.current( ) != i*10)
        {
            std::cout << "    Test failed to find " << i*10 << std::endl;
            return 0;
        }
        test.advance( );
    }
    if (test.is_item( ))
    {
        std::cout << "    There are too many items on the sequence." << std::endl;
        return false;
    }

    // All tests passed
    std::cout << "All tests of this second function have been passed." << std::endl;    
    return POINTS[2];
}


// **************************************************************************
// int test3( )
//   Performs basic tests for the remove_current function.
//   Returns POINTS[3] if the tests are passed. Returns POINTS[3] / 4 if almost
//   all the tests are passed. Otherwise returns 0.
// **************************************************************************
int test3( )
{
    // In the next declarations, I am declaring a sequence called test.
    // Both before and after the sequence, I declare a small array of characters,
    // and I put the character 'x' into each spot of these arrays.
    // Later, if I notice that one of the x's has been changed, or if
    // I notice an 'x' inside of the sequence, then the most
    // likely reason was that one of the sequence's member functions accessed
    // the sequence's array outside of its legal indexes.
    char prefix[4] = {'x', 'x', 'x', 'x'};
    sequence test;
    char suffix[4] = {'x', 'x', 'x', 'x'};

    // Within this function, I create several different sequences using the
    // items in these arrays:
    double items1[1] = { 30 };
    double items2[2] = { 10, 30 };
    double items3[3] = { 10, 20, 30 };
    
    size_t i;       // for-loop control variable
    char *char_ptr; // Variable to loop at each character in a sequence's memory

    // Build a sequence with three items 10, 20, 30, and remove the middle,
    // and last and then first.
    std::cout << "Using attach to build a sequence with 10,30." << std::endl;
    test.attach(10);
    test.attach(30);
    std::cout << "Insert a 20 before the 30, so entire sequence is 10,20,30." << std::endl;
    test.insert(20);
    if (!correct(test, 3, 1, items3)) return 0;
    std::cout << "Remove the 20, so entire sequence is now 10,30." << std::endl;
    test.start( );
    test.advance( );
    test.remove_current( );
    if (!correct(test, 2, 1, items2)) return 0;
    std::cout << "Remove the 30, so entire sequence is now just 10 with no cursor.";
    std::cout << std::endl;
    test.start( );
    test.advance( );
    test.remove_current( );
    if (!correct(test, 1, 1, items2)) return 0;
    std::cout << "Set the cursor to the start and remove the 10." << std::endl;
    test.start( );
    test.remove_current( );
    if (!correct(test, 0, 0, items2)) return 0;

    // Build a sequence with three items 10, 20, 30, and remove the middle,
    // and then first and then last.
    std::cout << "Using attach to build another sequence with 10,30." << std::endl;
    test.attach(10);
    test.attach(30);
    std::cout << "Insert a 20 before the 30, so entire sequence is 10,20,30." << std::endl;
    test.insert(20);
    if (!correct(test, 3, 1, items3)) return 0;
    std::cout << "Remove the 20, so entire sequence is now 10,30." << std::endl;
    test.start( );
    test.advance( );
    test.remove_current( );
    if (!correct(test, 2, 1, items2)) return 0;
    std::cout << "Set the cursor to the start and remove the 10," << std::endl;
    std::cout << "so the sequence should now contain just 30." << std::endl;
    test.start( );
    test.remove_current( );
    if (!correct(test, 1, 0, items1)) return 0;
    std::cout << "Remove the 30 from the sequence, resulting in an empty sequence." << std::endl;
    test.start( );
    test.remove_current( );
    if (!correct(test, 0, 0, items1)) return 0;

    // Build a sequence with three items 10, 20, 30, and remove the first.
    std::cout << "Build a new sequence by inserting 30, 10, 20 (so the sequence\n";
    std::cout << "is 20, then 10, then 30). Then remove the 20." << std::endl;
    test.insert(30);
    test.insert(10);
    test.insert(20);
    test.remove_current( );
    if (!correct(test, 2, 0, items2)) return 0;
    test.start( );
    test.remove_current( );
    test.remove_current( );

    // Just for fun, fill up the sequence, and empty it!
    std::cout << "Just for fun, I'll empty the sequence then fill it up, then\n";
    std::cout << "empty it again. During this process, I'll try to determine\n";
    std::cout << "whether any of the sequence's member functions access the\n";
    std::cout << "array outside of its legal indexes." << std::endl;
    for (i = 0; i < test.DEFAULT_CAPACITY; i++)
        test.insert(0);
    for (i = 0; i < test.DEFAULT_CAPACITY; i++)
        test.remove_current( );

    // Make sure that the character 'x' didn't somehow get into the sequence,
    // as that would indicate that the sequence member functions are
    // copying data from before or after the sequence into the sequence.
    char_ptr = (char *) &test;
    for (i = 0; i < sizeof(sequence); i++)
        if (char_ptr[i] == 'x')
        {
            std::cout << "Illegal array access detected." << std::endl;
            return POINTS[3] / 4;
        }

    // Make sure that the prefix and suffix arrays still have four
    // x's each. Otherwise one of the sequence operations wrote outside of
    // the legal boundaries of its array.
    for (i = 0; i < 4; i++)
        if ((suffix[i] != 'x') || (prefix[i] != 'x'))
        {
            std::cout << "Illegal array access detected." << std::endl;
            return POINTS[3] / 4;
        }
                                        
    // All tests passed
    std::cout << "All tests of this third function have been passed." << std::endl;    
    return POINTS[3];
}


// **************************************************************************
// int test4( )
//   Performs some tests of resize.
//   Returns POINTS[4] if the tests are passed. Otherwise returns 0.
// **************************************************************************
int test4( )
{
    sequence test;
    size_t i;
    char bytes[sizeof(sequence)];
    char newbytes[sizeof(sequence)];
    size_t mismatches;

    std::cout << "I will now resize a sequence to a larger capacity, and then\n";
    std::cout << "attach that many items. The sequence should NOT need to\n";
    std::cout << "resize itself under this situation." << std::endl;
    test.resize(2*test.DEFAULT_CAPACITY);
    test.attach(0);
    memcpy(bytes, (char *) &test, sizeof(sequence));
   
    // At this point, I should be able to insert 2*DEFAULT_CAPACITY-1
    // more items without calling resize again. Therefore, at most 1 byte
    // of the object will change (the least significant byte of used).

    for (i = 1; i < 2*test.DEFAULT_CAPACITY; i++)
        test.attach(i);
    test.start( );
    memcpy(newbytes, (char *) &test, sizeof(sequence));
     
    for (i = 0; i < 2*test.DEFAULT_CAPACITY; i++)
    {
        if (test.current( ) != i)
        {
            std::cout << "    sequence does not contain correct items." << std::endl;
            return 0;
        }
        test.advance( );
    }
    test.start( );
     
    mismatches = 0;
    for (i = 0; i < sizeof(sequence); i++)
        if (bytes[i] != newbytes[i])
            mismatches++;
    if (mismatches > 1)
    {
        std::cout << "    sequence was resized when it should not be." << std::endl;
        return 0;
    }
    else
        std::cout << "    Test passed." << std::endl;
     
    std::cout << "Now I will call resize(1) for the sequence, but the actual\n";
    std::cout << "sequence should not change because the sequence already has \n";
    std::cout << test.DEFAULT_CAPACITY*2 << " items." << std::endl;
    memcpy(bytes, (char *) &test, sizeof(sequence));
    test.resize(1);
    mismatches = 0;
    for (i = 0; i < sizeof(sequence); i++)
        if (bytes[i] != newbytes[i])
            mismatches++;
    if (mismatches > 0)
    {
        std::cout << "    sequence was resized when it should not be." << std::endl;
        return 0;
    }
    else
        std::cout << "    Test passed." << std::endl;

    // All tests passed
    std::cout << "All tests of this fourth function have been passed." << std::endl;    
    return POINTS[4];
}


// **************************************************************************
// int test5( )
//   Performs some tests of the copy constructor.
//   Returns POINTS[5] if the tests are passed. Otherwise returns 0.
// **************************************************************************
int test5( )
{
    sequence original; // A sequence that we'll copy.
    double items[2*original.DEFAULT_CAPACITY];
    size_t i;

    // Set up the items array to conatin 1...2*DEFAULT_CAPACITY.
    for (i = 1; i <= 2*original.DEFAULT_CAPACITY; i++)
        items[i-1] = i;
    
    // Test copying of an empty sequence. After the copying, we change the original.
    std::cout << "Copy constructor test: for an empty sequence." << std::endl;
    sequence copy1(original);
    original.attach(1); // Changes the original sequence, but not the copy.
    if (!correct(copy1, 0, 0, items)) return 0;

    // Test copying of a sequence with current item at the tail.
    std::cout << "Copy constructor test: for a sequence with cursor at tail." << std::endl;
    for (i=2; i <= 2*original.DEFAULT_CAPACITY; i++)
        original.attach(i);
    sequence copy2(original);
    original.start( );
    original.advance( );
    original.remove_current( ); // Removes 2 from the original, but not the copy.
    if (!correct
        (copy2, 2*original.DEFAULT_CAPACITY, 2*original.DEFAULT_CAPACITY-1, items)
        )
        return 0;

    // Test copying of a sequence with cursor near the middle.
    std::cout << "Copy constructor test: for a sequence with cursor near middle." << std::endl;
    original.insert(2);
    for (i = 1; i < original.DEFAULT_CAPACITY; i++)
        original.advance( );
    // Cursor is now at location [DEFAULT_CAPACITY] (counting [0] as the first spot).
    sequence copy3(original);
    original.start( );
    original.advance( );
    original.remove_current( ); // Removes 2 from the original, but not the copy.
    if (!correct
        (copy3, 2*original.DEFAULT_CAPACITY, original.DEFAULT_CAPACITY, items)
        )
        return 0;

    // Test copying of a sequence with cursor at the front.
    std::cout << "Copy constructor test: for a sequence with cursor near middle." << std::endl;
    original.insert(2);
    original.start( );
    // Cursor is now at the front.
    sequence copy4(original);
    original.start( );
    original.advance( );
    original.remove_current( ); // Removes 2 from the original, but not the copy.
    if (!correct
        (copy4, 2*original.DEFAULT_CAPACITY, 0, items)
        )
        return 0;

    // Test copying of a sequence with no current item.
    std::cout << "Copy constructor test: for a sequence with no current item." << std::endl;
    original.insert(2);
    while (original.is_item( ))
        original.advance( );
    // There is now no current item.
    sequence copy5(original);
    original.start( );
    original.advance( );
    original.remove_current( ); // Removes 2 from the original, but not the copy.
    if (!correct
        (copy5, 2*original.DEFAULT_CAPACITY, 2*original.DEFAULT_CAPACITY, items)
        )
        return 0;

    // All tests passed
    std::cout << "All tests of this fifth function have been passed." << std::endl;    
    return POINTS[5];
} 


// **************************************************************************
// int test6( )
//   Performs some tests of the assignment operator.
//   Returns POINTS[6] if the tests are passed. Otherwise returns 0.
// **************************************************************************
int test6( )
{
    sequence original; // A sequence that we'll copy.
    double items[2*original.DEFAULT_CAPACITY];
    size_t i;

    // Set up the items array to conatin 1...2*DEFAULT_CAPACITY.
    for (i = 1; i <= 2*original.DEFAULT_CAPACITY; i++)
    {
        items[i-1] = i;
        std::cout << "One    " << items[i-1] << std::endl;
    }
    // Test copying of an empty sequence. After the copying, we change the original.
    std::cout << "Assignment operator test: for an empty sequence." << std::endl;
    sequence copy1;
    copy1 = original;
    original.attach(1); // Changes the original sequence, but not the copy.
    if (!correct(copy1, 0, 0, items)) return 0;
    for (i = 1; i <= 2*original.DEFAULT_CAPACITY; i++)
    {
        //items[i-1] = i;
        std::cout << "Two    " << items[i-1] << std::endl;
    }
    // Test copying of a sequence with current item at the tail.
    std::cout << "Assignment operator test: for a sequence with cursor at tail." << std::endl;
    for (i=2; i <= 2*original.DEFAULT_CAPACITY; i++)
        original.attach(i);
    sequence copy2;
    copy2 = original;
    original.start( );
    original.advance( );
    original.remove_current( ); // Removes 2 from the original, but not the copy.
    if (!correct
        (copy2, 2*original.DEFAULT_CAPACITY, 2*original.DEFAULT_CAPACITY-1, items)
        )
        return 0;
    for (i = 1; i <= 2*original.DEFAULT_CAPACITY; i++)
    {
        //items[i-1] = i;
        std::cout << "Three    " << items[i-1] << std::endl;
    }
    // Test copying of a sequence with cursor near the middle.
    std::cout << "Assignment operator test: for a sequence with cursor near middle." << std::endl;
    original.insert(2);
    for (i = 1; i < original.DEFAULT_CAPACITY; i++)
        original.advance( );
    // Cursor is now at location [DEFAULT_CAPACITY] (counting [0] as the first spot).
    sequence copy3;
    copy3 = original;
    original.start( );
    original.advance( );
    original.remove_current( ); // Removes 2 from the original, but not the copy.
    if (!correct
        (copy3, 2*original.DEFAULT_CAPACITY, original.DEFAULT_CAPACITY, items)
        )
        return 0;
    for (i = 1; i <= 2*original.DEFAULT_CAPACITY; i++)
    {
        //items[i-1] = i;
        std::cout << "Four    " << items[i-1] << std::endl;
    }
    // Test copying of a sequence with cursor at the front.
    std::cout << "Assignment operator test: for a sequence with cursor near middle." << std::endl;
    original.insert(2);
    original.start( );
    // Cursor is now at the front.
    sequence copy4;
    copy4 = original;
    original.start( );
    original.advance( );
    original.remove_current( ); // Removes 2 from the original, but not the copy.
    if (!correct
        (copy4, 2*original.DEFAULT_CAPACITY, 0, items)
        )
        return 0;
    for (i = 1; i <= 2*original.DEFAULT_CAPACITY; i++)
    {
        //items[i-1] = i;
        std::cout << "Five    " << items[i-1] << std::endl;
    }
    // Test copying of a sequence with no current item.
    std::cout << "Assignment operator test: for a sequence with no current item." << std::endl;
    original.insert(2);
    while (original.is_item( ))
        original.advance( );
    // There is now no current item.
    sequence copy5;
    copy5 = original;
    original.start( );
    original.advance( );
    original.remove_current( ); // Removes 2 from the original, but not the copy.
    if (!correct
        (copy5, 2*original.DEFAULT_CAPACITY, 2*original.DEFAULT_CAPACITY, items)
        )
        return 0;
    for (i = 1; i <= 2*original.DEFAULT_CAPACITY; i++)
    {
        //items[i-1] = i;
        std::cout << "Six    " << items[i-1] << std::endl;
    }
    std::cout << "Checking correctness of a self-assignment x = x;" << std::endl;
    original.insert(2);
    original = original;
    if (!correct
        (original, 2*original.DEFAULT_CAPACITY, 1, items)
        )
        return 0;
    for (i = 1; i <= 2*original.DEFAULT_CAPACITY; i++)
    {
        //items[i-1] = i;
        std::cout << "Eight    " << items[i-1] << std::endl;
    }
    // All tests passed
    std::cout << "All tests of this sixth function have been passed." << std::endl;    
    return POINTS[6];
} 


// **************************************************************************
// int test7( )
//   Performs some basic tests of insert and attach for the case where the
//   current capacity has been reached.
//   Returns POINTS[7] if the tests are passed. Otherwise returns 0.
// **************************************************************************
int test7( )
{
    sequence testa, testi;
    double items[2*testa.DEFAULT_CAPACITY];
    size_t i;

    // Set up the items array to conatin 1...2*DEFAULT_CAPACITY.
    for (i = 1; i <= 2*testa.DEFAULT_CAPACITY; i++)
        items[i-1] = i;
    
    std::cout << "Testing to see that attach works correctly when the\n";
    std::cout << "current capacity is exceeded." << std::endl;
    for (i = 1; i <= 2*testa.DEFAULT_CAPACITY; i++)
        testa.attach(i);
    if (!correct
        (testa, 2*testa.DEFAULT_CAPACITY, 2*testa.DEFAULT_CAPACITY-1, items)
        )
        return 0;

    std::cout << "Testing to see that insert works correctly when the\n";
    std::cout << "current capacity is exceeded." << std::endl;
    for (i = 2*testi.DEFAULT_CAPACITY; i >= 1; i--)
        testi.insert(i);
    if (!correct
        (testi, 2*testi.DEFAULT_CAPACITY, 0, items)
        )
        return 0;

    // All tests passed
    std::cout << "All tests of this seventh function have been passed." << std::endl;    
    return POINTS[7];
}  

int run_a_test(int number, const char message[], int test_function( ), int max)
{
    int result;
    
    std::cout << std::endl << "START OF TEST " << number << ":" << std::endl;
    std::cout << message << " (" << max << " points)." << std::endl;
    result = test_function( );
    if (result > 0)
    {
        std::cout << "Test " << number << " got " << result << " points";
        std::cout << " out of a possible " << max << "." << std::endl;
    }
    else
        std::cout << "Test " << number << " failed." << std::endl;
    std::cout << "END OF TEST " << number << "." << std::endl << std::endl;
    
    return result;
}

/*int test8(){
    std::cout << "Test 8 is now beginning..." << std::endl;
    sequence testb, testw;
    testb.start();
    testw.start();
    testb.insert(4);
    testb.insert(10);
    testb.insert(6);
    testw = testb;
    std::cout << "Testing to see if testw == testb..." << std::endl;
    if (!(testw == testb))
    {
        return 0;
    }
    std::cout << "Passed." << std::endl;
    std::cout << "Testing to see if testw != testb..." << std::endl;
    if (testw != testb)
    {
        return 0;
    }
    std::cout << "Passed." << std::endl;
    std::cout << "All tests of this eighth function have been passed." << std::endl;
    return POINTS[8];
}*/

// **************************************************************************
// int main( )
//   The main program calls all tests and prints the sum of all points
//   earned from the tests.
// **************************************************************************
int main( )
{
    int sum = 0;
    
    
    std::cout << "Running " << DESCRIPTION[0] << std::endl;

    sum += run_a_test(1, DESCRIPTION[1], test1, POINTS[1]);
    sum += run_a_test(2, DESCRIPTION[2], test2, POINTS[2]);
    sum += run_a_test(3, DESCRIPTION[3], test3, POINTS[3]);
    sum += run_a_test(4, DESCRIPTION[4], test4, POINTS[4]);
    sum += run_a_test(5, DESCRIPTION[5], test5, POINTS[5]);
    sum += run_a_test(6, DESCRIPTION[6], test6, POINTS[6]);
    sum += run_a_test(7, DESCRIPTION[7], test7, POINTS[7]);
    //sum += run_a_test(8, DESCRIPTION[8], test8, POINTS[8]);

    std::cout << "If you submit this sequence now, you will have\n";
    std::cout << sum << " percent out of the " << POINTS[0];
    std::cout << " percent from this test program.\n";
    
    return EXIT_SUCCESS;

}
