// Provided by:   Marcus Hershberger
// Lab:           180
// Course number: 221
// Besides the code that was provided for the assignment, I typed all of the code.

#include "sequence.h"
#include <cstdlib>
#include <string>
#include <iostream>
#include <algorithm>
#include <cassert>
 
namespace main_savitch_4
{ 
    const sequence::size_type sequence::DEFAULT_CAPACITY;
    
    sequence::sequence(size_type initial_capacity)
    {
        data_ = new value_type[initial_capacity];
        used_ = 0;
        capacity_ = initial_capacity;
        current_index_ = 0;
    }
    
    sequence::sequence(const sequence& source)
    {
        data_ = new value_type[source.capacity_];
        capacity_ = source.capacity_;
        used_ = source.used_;
        std::copy(source.data_, source.data_+used_, data_);
        current_index_ = source.current_index_;
    }
    
    sequence::~sequence( )
    {
        delete [] data_;
    }
    
    void sequence::resize(size_type new_capacity)
    {
        value_type *bigger_array_;
        
        if (new_capacity == capacity_)
        {
            return;
        }
        if (new_capacity < used_)
        {
            new_capacity = used_;
        }
        
        bigger_array_ = new value_type[new_capacity];
        std::copy(data_, data_+used_, bigger_array_);
        delete [] data_;
        data_ = bigger_array_;
        capacity_ = new_capacity;
        
    }
    
    void sequence::start()
    {
        current_index_ = 0;
    }
    
    void sequence::advance()
    {
        current_index_ += 1;
    }
    
    void sequence::insert(const value_type& entry) 
    {
        if (used_ >= capacity_)
        {
            resize((capacity_ + 1) * 2);
        }
        if (!is_item())
        {
            current_index_ = 0;
        }
        if (used_ > 0)
        {
            for (int i = used_ - 1; i >= (int)current_index_; i--){
                data_[i + 1] = data_[i];
            }
        }
        data_[current_index_] = entry;
        used_ += 1;
    }
    
    void sequence::attach(const value_type& entry)
    {
        
        if (used_ >= capacity_)
        {
            resize((capacity_ + 1)*2);
        }
        if (!is_item() && used_ >= 1)
        {
            current_index_ = 0;
            while (current_index_ < used_ - 1)
            {
                advance();
            }
        }
        if (used_ > 0)
        {
            for (int i = used_ - 1; i > (int)current_index_; i--){
                data_[i + 1] = data_[i];
            }
            data_[current_index_ + 1] = entry;
            current_index_ += 1;
            used_ += 1;
        }
        else
        {
            data_[current_index_] = entry;
            used_ += 1;
        }
    }
    
    void sequence::remove_current()
    {
        assert(size() > 0);
        for (size_type i = current_index_ + 1; i < used_; i++)
        {
            data_[i - 1] = data_[i];
        }
        used_-=1;
    }
    
    bool sequence::is_item()const
    {
        return !(current_index_ >= used_);
    }
    
    sequence::size_type sequence::size()const
    {
        return used_;
    }
    
    sequence::value_type sequence::current()const
    {
        assert (is_item());
        return data_[current_index_];
    }
    
    void sequence::operator =(const sequence& source)
    {
        value_type *new_data;
        if (this == &source)
            return;
        if (capacity_ != source.capacity_)
        {
            new_data = new value_type[source.capacity_];
            delete [] data_;
            data_ = new_data;
            capacity_ = source.capacity_;
        }
        
        used_ = source.used_;
        current_index_ = source.current_index_;
        std::copy(source.data_, source.data_+used_, data_);
    }
    
    bool sequence::operator ==(const sequence& other)
    {
        if (used_ == 0 && other.used_ == 0)
        {
            return true;
        }
        else if (used_ > 0 && used_ == other.used_)
        {
            int differences = 0;
            for (int i = 0; i < (int)used_; i++)
            {
                if (this->data_[i] == other.data_[i])
                {
                    differences++;
                }
            }
            return (differences == 0);
        }
        else
        {
            return false;
        }
    }
    
    bool sequence::operator !=(const sequence& other)
    {
        if (used_ == 0 && other.used_ == 0)
        {
            return false;
        }
        else if (used_ != other.used_)
        {
            return true;
        }
        else
        {
            int differences = 0;
            for (int i = 0; i < (int)used_; i++)
            {
                if (data_[i] != other.data_[i])
                {
                    differences++;
                }
            }
            return (differences > 0);
        }
    }
}