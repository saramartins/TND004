using namespace std;


template<typename T>
class Set
{

public:
    Set(); // Default constructor
    Set(const Set &s); // Copy constructor
    ~Set() = default; // Destructor
    Set(T value); // Conversion constructor from T to Set
    Set(Set &&R) noexcept; // Move constructor (noexcept to make sure it uses move instead of copy)

    Set(T a[], int n);
    const Set &operator=(const Set &R); // Assignment operator
    Set &operator=(Set &&R) noexcept;  // Move Assignment operator
    bool _empty() const; // Function to test if Set is empty
    int cardinality() const; // How many elements in the Set
    bool is_member(T value) const; // Test if v belongs to the Set
    void make_empty(); // set the Set to empty
    void print(ostream& os) const;
    void init();

    // Mixed mode arithmetic expressions should be supported:
    // R+k, k+R, R*k etc where R is a set of elements of some type T
    // and k is a a value of type T

    // Overloaded operators
    Set operator+=(const Set &R);
    Set operator*=(const Set &R);
    Set operator-=(const Set &R);

    // Comparison operators
    bool operator<=(const Set &R) const;
    bool operator<(const Set &R) const;
    bool operator==(const Set &R) const;
    bool operator!=(const Set &R) const; //returns true if R(S) has an element that does not belong to S(R)

private:

    struct Node {
        T data;
        shared_ptr<Node> prev;
        shared_ptr<Node> next;


        Node(const T &d = T{}, shared_ptr<Node> p = nullptr, shared_ptr<Node> n = nullptr)
            : data{d}, prev{p}, next{n} { }

        Node(T &&d, shared_ptr<Node> p = nullptr, shared_ptr<Node> n = nullptr)
            : data{ move(d)}, prev{p}, next{n} { }

        ~Node() = default;

        // Destructor deallocating the nodes in the list
        // including the dummy nodes

    };

    friend ostream& operator<<(ostream& os, const Set<T> &R){

        if(R._empty())
            os << "Set is empty!";
        else
            R.print(os);

        return os;
    }

    friend Set operator+( Set<T> &R1, const Set<T> &R2){
        return (R1 += R2);
    }
    friend Set operator*(const Set<T> &R1, const Set<T> &R2) {
        return (R1 *= R2);
    }
    friend Set operator-(const Set<T> &R1, const Set<T> &R2){
        return (R1 -= R2);
    }

    int theSize;
    shared_ptr<Node> head;
    shared_ptr<Node> tail;
    unsigned counter;


public:

    Set& _insert(const shared_ptr<Node>& p, const T& val);

    Set& _remove(const shared_ptr<Node>& p);

};

//default constructor
template<typename T>
Set<T>::Set()
{
    init();
}

//copy constructor
template<typename T>
Set<T>::Set(const Set &s)
{
    init();

    auto tmp = s.tail->prev;

    while(tmp->prev)
    {
        _insert(head, tmp->data);
        tmp = tmp->prev;
    }

}

//conversion constructor
template<typename T>
Set<T>::Set(T value)
{
    init();
    _insert(head, value);
}

//move constructor
template<typename T>
Set<T>::Set(Set &&R) noexcept
:counter{R.counter}, head{R.head}, tail{R.tail}
    {
    cout << "Move constructor ..." << endl;
    R.counter = 0;
    R.head = nullptr;
    R.tail = nullptr;

}

// create set from array
template<typename T>
Set<T>::Set(T a[], int n)
{
    init();

    auto temp = head;
    for(int i = 0; i < n; i++)
    {
        _insert(temp, a[i]);
        temp = temp->next;
    }
}

template<typename T>
void Set<T>::init(){

    counter = 0;
    head = make_shared<Node>();
    tail = make_shared<Node>();
    head->next = tail;
    tail->prev = head;

}

/************ MASSA OPERATORS *************/

template<typename T>// Assignment operator
const Set<T>& Set<T>::operator=(const Set &R){

    if(this != &R){

        Set _copy(R);

        swap(counter, _copy.counter);
        swap(head, _copy.head);
        swap(tail,_copy.tail);

    }

    return *this;
}


template<typename T>// Move Assignment operator
Set<T>& Set<T>::operator=(Set &&R) noexcept{

    swap(counter, R.counter);
    swap(head, R.head);
    swap(tail, R.tail);

    return *this;

}

// Overloaded operators
// Not working! Union
template<typename T>
Set<T> Set<T>::operator+=(const Set &R) {

    auto p1 = head->next;
    auto p2 = R.head->next;

    while(p1 != tail && p2 != tail)
    {
       if(p1->data < p2->data){
            cout << p1->data << "<" << p2->data << endl;
            p1 = p1->next;

       }
       else if(p1->data > p2->data){
            _insert(p1->prev, p2->data);
             cout << p1->data << ">" << p2->data << endl;
            p2 = p2->next;
       }
       else{
        //lika
        p1=p1->next;
        p2=p2->next;
       }

    }
    while(p2!=tail){
       // _insert(tail->prev, p2->data);
        p2= p2->next;
    }


    return *this;

}


//this one is working but we want to get rid of the Set c(*this) and only use this.
template<typename T>
Set<T> Set<T>::operator*=(const Set &R) {

    Set c(*this);
    auto p1 = head->next;

    while(p1 != tail)
    {
        if(!R.is_member(p1->data))
        {
            c._remove(p1);
        }
        p1 = p1->next;
    }

    return c;
}

// difference
//this one is not working, we think we should do same as above but inverse
template<typename T>
Set<T> Set<T>::operator-=(const Set &R) {

    cout << "HALOOO!";

    auto p1 = head->next;

    while(p1 != tail)
    {
        cout << "inne i while";

        if(!R.is_member(p1->data))
        {
            cout << "is member: " << p1->data << endl;
            //_remove(p1);
        }

        p1 = p1->next;
        cout << "hej" <<endl;
    }

    return *this;
}

    // Comparison operators
template<typename T>
bool Set<T>::operator<=(const Set &R) const{

    auto tmp = head->next;

    while(tmp != tail)
    {
        if(!R.is_member(tmp->data)) // om tmp->data inte finns i R,
            return false;

        tmp = tmp->next;
    }

    return true;
}

template<typename T>
bool Set<T>::operator<(const Set &R) const{
    return false;
}

template<typename T>
bool Set<T>::operator==(const Set &R) const{

    auto tmp = head->next;
    auto S = R.head->next;

    while(tmp != tail)
    {
        if( (tmp->data <= S->data) && (S->data <= tmp->data) )
            return true;
        else
            return false;

            tmp = tmp->next;
            S = S->next;
    }

}

template<typename T> //returns true if R(S) has an element that does not belong to S(R)
bool Set<T>::operator!=(const Set &R) const{
    return false;
}

/*********** MASSA FUNKTIONER ************/


template<typename T>
Set<T>& Set<T>::_remove(const shared_ptr<Node>& p)
{
    p->prev->next = p->next;
    p->next->prev = p->prev;
    //p = nullptr;
    counter--;

    return *this;
}

template<typename T>
Set<T>& Set<T>::_insert(const shared_ptr<Node>& p, const T& val)
{

    auto newNode = make_shared<Node>(val, p, p->next);
    p->next->prev = newNode;
    p->next = newNode;

    ++counter;

    return *this;

}

template<typename T>// Function to test if Set is empty
bool Set<T>::_empty() const{

    if(head->next == tail)
        return true;
    else
        return false;
}

template<typename T>// How many elements in the Set
int Set<T>::cardinality() const{

    return counter;
}

template<typename T>// Test if value belongs to the Set
bool Set<T>::is_member(T value) const{

    auto tmp = tail->prev;
    while(tmp->prev){
        if(tmp->data == value){
            return true;
        }

        tmp = tmp->prev;
    }

    return false;
}

template<typename T>// set the Set to empty
void Set<T>::make_empty(){

    auto tmp = head->next;

    while(tmp->next){
        tmp = tmp->next;
        tmp->prev = nullptr;

        counter--;
    }

}

//Display all elements of a non-empty list
template<typename T>
void Set<T>::print(ostream& os) const
{
    shared_ptr<Node> temp = head->next;

    os << "{ ";
    while ( temp->next )
    {
        os << temp->data << " ";
        temp = temp->next;
    }

    os << "}" << endl;
}


