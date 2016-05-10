
/*
  Author: Aida Nordman
  Course: TND004, Lab 2
  Description: template class HashTable represents an open addressing hash table
              (also known as closed_hashing) with linear probing
*/

#include "Item.h"

#include <iostream>
#include <iomanip>

using namespace std;

const int NOT_FOUND = -1;
const double MAX_LOAD_FACTOR = 0.5;


//Template class to represent an open addressing hash table using linear probing to resolve collisions
//Internally the table is represented as an array of pointers to Items
template <typename Key_Type, typename Value_Type>
class HashTable
{
public:

    //New type HASH: pointer to a hash function
    // HASH pekar på en funktion, kommer returnera en unsigned som är mellan 0 och strl på listan. Ger positionen där vi vill börja vår linear probing
    // Använder h för att göra "hashningen".
    typedef unsigned (*HASH)(Key_Type, int);


    //Constructor to create a hash table
    //table_size is number of slots in the table (next prime number is used)
    //f is the hash function
    HashTable(int table_size, HASH f);  // OK!


    //Destructor
    ~HashTable();   // OK!


    //Return the load factor of the table, i.e. percentage of slots in use or deleted
    double loadFactor() const   // OK!
    {
        return ((double) (nItems+nDeleted)) / _size;
    }


    //Return number of items stored in the table
    unsigned get_number_OF_items() const    // OK!
    {
        return nItems;
    }

    //Return the total number of visited slots (during search, insert, remove, or re-hash)
    unsigned get_total_visited_slots() const    // OK!
    {
        return total_visited_slots;
    }

    //Return the total number of call to new Item()
    unsigned get_count_new_items() const    // OK!
    {
        return count_new_items;
    }

    unsigned get_nr_of_rehash() const
    {
        return nr_of_rehash;
    }

    //Return a pointer to the value associated with key
    //If key does not exist in the table then nullptr is returned
    const Value_Type* _find(const Key_Type& key);   // OK!
    int findValue(const Key_Type& key);    //skapat själva OK


    //Insert the Item (key, v) in the table
    //If key already exists in the table then change the value associated with key to v
    //Re-hash if the table reaches the MAX_LOAD_FACTOR
    void _insert(const Key_Type& key, const Value_Type& v); // OK


    //Remove Item with key, if the item exists
    //If an Item was removed then return true
    //otherwise, return false
    bool _remove(const Key_Type& key);  // OK

    //Overloaded subscript operator
    //If key is not in the table then insert a new Item = (key, Value_Type())
    Value_Type& operator[](const Key_Type& key); // OK


    //Display all items in table T to stream os
    friend ostream& operator<<(ostream& os, const HashTable& T)
    {
        for (unsigned i = 0; i < T._size; ++i)
        {
            if (T.hTable[i] && T.hTable[i] != Deleted_Item<Key_Type,Value_Type>::get_Item())
            {
                os << *T.hTable[i] << endl;
            }
        }

        return os;
    }


    //Display the table for debug and testing purposes
    //Thus, empty and deleted entries are also displayed
    void display(ostream& os);

    void reHash();  // OK

private:

    /* ********************************** *
    * Data members                        *
    * *********************************** */

    //Number of slots in the table, a prime number
    unsigned _size;

    //Hash function
    const HASH h;

    //Number of items stored in the table
    //Instances of Deleted_Items are not counted
    unsigned nItems;

    //Number of slots that are marked as deleted
    unsigned nDeleted;

    //Table is an array of pointers to Items
    //Each slot of the table stores a pointer to an Item =(key, value)
    Item<Key_Type, Value_Type>** hTable;

    //Some statistics
    unsigned total_visited_slots;  //total number of visited slots
    unsigned count_new_items;      //number of calls to new Item()
    unsigned nr_of_rehash;


    /* ********************************** *
    * Auxiliar member functions           *
    * *********************************** */

    //Disable copy constructor!!
    HashTable(const HashTable &) = delete;

    //Disable assignment operator!!
    const HashTable& operator=(const HashTable &) = delete;

};


//Test if a number is prime
bool isPrime( int n );

//Return a prime number at least as large as n
int nextPrime( int n );


/* ********************************** *
* Member functions implementation     *
* *********************************** */

//Constructor to create a hash table
//table_size number of slots in the table (next prime number is used)
//f is the hash function
template <typename Key_Type, typename Value_Type>
HashTable<Key_Type, Value_Type>::HashTable(int table_size, HASH f)
    : h(f)
{
    //IMPLEMENT
    count_new_items = 0;
    total_visited_slots = 0;
    nDeleted = 0;
    nItems = 0;
    nr_of_rehash = 0;
    //Return a prime number at least as large as n

    _size = nextPrime(table_size);
    hTable = new Item<Key_Type,Value_Type>*[_size];
    count_new_items++;

    for(unsigned i = 0; i < _size; i++){
        hTable[i] = nullptr;

    }


}


//Destructor
template <typename Key_Type, typename Value_Type>
HashTable<Key_Type, Value_Type>::~HashTable()
{
    //IMPLEMENT
    for(unsigned i = 0; i < _size; i++)
    {

        if(hTable[i] != nullptr)
        {
            --nItems;
            delete hTable[i];

        }
        delete [] hTable;
    }


}


//Return a pointer to the value associated with key
//If key does not exist in the table then nullptr is returned
template <typename Key_Type, typename Value_Type>
const Value_Type* HashTable<Key_Type, Value_Type>::_find(const Key_Type& key)
{
        //IMPLEMENT
        int place = findValue(key);
        if(hTable[place]){
            total_visited_slots++;
            return &(hTable[place]->get_value());
        }
        else {
            return nullptr;

        }
}


template <typename Key_Type, typename Value_Type>
int HashTable<Key_Type, Value_Type>::findValue(const Key_Type& key)
{

    unsigned i = h(key, _size);

    //++total_visited_slots;
    while((hTable[i] != nullptr && hTable[i]->get_key() != key) || hTable[i] == Deleted_Item<Key_Type,Value_Type>::get_Item()) {
        i++;
        i %= _size;
        ++total_visited_slots;  //total number of visited slots
    }

    return i;
}


//Insert the Item (key, v) in the table
//If key already exists in the table then change the value associated with key to v
//Re-hash if the table reaches the MAX_LOAD_FACTOR
template <typename Key_Type, typename Value_Type>
void HashTable<Key_Type, Value_Type>::_insert(const Key_Type& key, const Value_Type& v)
{
     //IMPLEMENT

    if(loadFactor()>MAX_LOAD_FACTOR)
        reHash();


     int place = findValue(key);
     if(!hTable[place])
     {
         hTable[place] = new Item<Key_Type,Value_Type>(key, v);
         ++nItems;
         count_new_items++;


     }
     else
     {
         hTable[place]->set_value(v);
         nItems++;


     }



     // på den plats som find() misslyckas ska insert stoppa in värdet

}
template <typename Key_Type, typename Value_Type>
void HashTable<Key_Type,Value_Type>::reHash()
{
    auto old_size = _size;
    auto old_table = hTable;
    nItems = 0;

    cout << "Rehashing..." << endl;
    _size = nextPrime(_size * 2);
    cout << "New size = " << _size << endl;

    // Skapa ett helt nytt table med ny strl
    hTable = new Item<Key_Type,Value_Type>*[_size];

    ++nr_of_rehash;

    // sätt den nya hTable till nollställd
    for(unsigned i = 0; i < _size; i++)
    {
        hTable[i] = nullptr;
    }

  //  unsigned index = h(key, _size);

    // kopiera över gamla värden till nya tablet.
    for(unsigned i = 0; i < old_size; i++)
    {
        //++total_visited_slots;
        if(old_table[i] != nullptr && old_table[i] != Deleted_Item<Key_Type,Value_Type>::get_Item())
        {
            unsigned index = findValue(old_table[i]->get_key());
            hTable[index] = old_table[i];
            nItems++;
            total_visited_slots++;
        }

    }

}

//Remove Item with key, if the item exists
//If an Item was removed then return true
//otherwise, return false
template <typename Key_Type, typename Value_Type>
bool HashTable<Key_Type, Value_Type>::_remove(const Key_Type& key)
{
    //IMPLEMENT
    int place = findValue(key);

    if(hTable[place])
     {
        total_visited_slots++;
         --nItems;
         hTable[place] = Deleted_Item<Key_Type,Value_Type>::get_Item();

     }

    ++nDeleted;
    return true;
}


//Display the table for debug and testing purposes
//This function is used for debugging and testing purposes
//Thus, empty and deleted entries are also displayed
template <typename Key_Type, typename Value_Type>
void HashTable<Key_Type, Value_Type>::display(ostream& os)
{
    os << "-------------------------------\n";
    os << "Number of items in the table: " << get_number_OF_items() << endl;
    os << "Load factor: " << fixed << setprecision(2) << loadFactor() << endl;

    for (unsigned i = 0; i < _size; ++i)
    {
        os << setw(6) << i << ": ";

        if (!hTable[i])
        {
            os << "null" << endl;
        }
        else if (hTable[i] == Deleted_Item<Key_Type, Value_Type>::get_Item())
        {
            os << "deleted" << endl;
        }
        else
        {
            os << *hTable[i]
               << "  (" << h(hTable[i]->get_key(),_size) << ")" << endl;
        }
    }

    os << endl;
}



//Overloaded subscript operator
//If key is not in the table then insert a new Item = (key, Value_Type())
template <typename Key_Type, typename Value_Type>
Value_Type& HashTable<Key_Type, Value_Type>::operator[](const Key_Type& key)
{

     int place = findValue(key);
     if(!hTable[place])
     {
         hTable[place] = new Item<Key_Type,Value_Type>(key, Value_Type{});
         ++nItems;
         count_new_items++;
         total_visited_slots++;
     }



     if(loadFactor()>MAX_LOAD_FACTOR) {
        Value_Type& tmp = hTable[place]->get_value();
        reHash();

        return tmp;
     }

    return hTable[place]->get_value();

}


/* ********************************** *
* Auxiliar member functions           *
* *********************************** */
//Add any if needed


/* ********************************** *
* Functions to find prime numbers     *
* *********************************** */


//Test if a number is prime
bool isPrime( int n )
{
    if( n == 2 || n == 3 )
        return true;

    if( n == 1 || n % 2 == 0 )
        return false;

    for( int i = 3; i * i <= n; i += 2 )
        if( n % i == 0 )
            return false;

    return true;
}


//Return a prime number at least as large as n
int nextPrime( int n )
{
    if( n % 2 == 0 )
        n++;

    for(; !isPrime( n ); n += 2 );

    return n;
}


