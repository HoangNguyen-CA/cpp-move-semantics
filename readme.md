

# CPP Move Semantics

  

This is a small project that I built to get a deeper insight into C++ optimizations. In this readme, I documented my findings about the topic.

## Table of Contents
- [Introduction](#introduction)
- [Resource Class](#resource-class)
	 -  [Rule of 5](#rule-of-5)
	 -  [Rule of 4.5](#rule-of-4.5)
- [Q&A](#q-and-a)
- [Program Outputs And Explanation](#program-outputs-and-explanation)
	- [Constructor Outputs](#constructor-outputs) 
	- [Assignment Outputs](#assignment-outputs) 


## Introduction
Move semantics is a built-in feature of C++ to handle efficient transfer of ownership. It is an alternative to copying an object and should be used when the cost of copying is high. 

## Resource Class

In my examples, I use a Resource class, which dynamically allocates an array of integers and holds a pointer to the start of the array. 

Since we need to clean up the dynamic data, we need a destructor to clean up the data

 ```cpp
Resource::~Resource(){
	if (m_data  !=  nullptr)
	{
		delete[]  m_data;
	}
}
```

### Rule of 5
By defining a destructor, adherence to the Rule of 5 becomes necessary. This rule prescribes a set of methods, implying that when one is specified, all five methods should be defined.

* Destructor
* Copy Constructor
* Copy Assignment
* Move Constructor
* Move Assignment

 Why is this the case? this rule helps prevent memory misuse when copying or moving the resource:
```cpp
Resource r1(); // holds int* 
Resource r2 = r1; // shallow copy r1 int*, both resources have same pointer

// r1 goes out of scope... int* gets deallocated by destructor
// r2 goes out of scope... r2 holds a dangling pointer and is deallocated... undefined behaviour
```

 By defining a copy constructor/copy assignment, we can deep copy the resource so that both of the objects don't point to the same location:
```cpp
// Example of copy constructor
Resource::Resource(const  Resource  &r)
{
	m_size  =  r.m_size;
	m_data  =  new  int[m_size]; // allocates own memory instead of shallow copy
}
```
Notice that the cost of copying is high since we have to allocate an array of size n for every copy. That is why we should also define a move constructor/move assignment. 

Moving a Resource r1 into r2 means that we are giving r2 control of r1 members while leaving r1 in a valid but indeterminate state. r2 essentially steals from r1, which means that no memory needs to be allocated.

A move implicitly happens when the item being passed into a function is an rvalue (temporary value) and the item defines the move constructor/assignment. 

```cpp
// Example of move constructor, we see that the resource steals the m_data pointer from the other while leaving the 'other' object in an empty state. No allocation is needed.
Resource(Resource&& other) {
 m_data = other.m_data; 
 m_size = other.m_size; 
 // Reset the source 
 other.m_data= nullptr; 
 other.m_size = 0; 
}
```

The copy assignment and move assignment operators are defined similarly.
### Rule of 4.5

We can make improvements on the rule of 5 by using the copy-swap idiom. We need to define a swap method in the class to handle member wise swapping (std::swap is not suitable since it causes infinite recursion with copy methods). 

```cpp
void  Resource::swap(Resource  &a, Resource  &b)
{
	std::swap(a.m_size, b.m_size);
	std::swap(a.m_data, b.m_data);
}
```

Thanks to this swap method, we can combine the copy assignment and move assignment overloads into one method.

```cpp
Resource  &Resource::operator=(Resource  r)
{
	swap(*this, r);
	return  *this;
}
```

How does this handle a move? **C++ implicitly moves a resource into a pass-by-value parameter when it is an rvalue.**

If the object is moved, we will steal/swap from it and leave it in a valid state (since our object is in a valid state). 

If the object is copied, we will steal/swap from the copied resource, which will be destroyed at the end of the function anyway.

Thanks to the swap function, we can also redefine the move constructor:

```cpp
Resource::Resource(Resource  &&other)
{	
	swap(*this, other);
}
```
In the move constructor, we swap `*this` with `other`, note that `*this` is uninitialized, but being uninitialized is considered to be in a valid state. **If we want `other` to be left in a default state, we can call the default constructor before the swap.** Normally, it is not advised to do this since `other` is often not used after the move.

This simplifies our code greatly:
* Custom destructor logic = 1
* Custom copy constructor logic = 2
* Swap move constructor logic = 3
* Swap assignment logic = 4
* Swap helper = 4.5

## Q and A

Q: When are the default move methods defined? 
A: When no destructor is defined, and no copy methods are defined, then the default move methods will be defined.

Q: What do the default move methods do?
A: The default move methods perform a move on all class members. 

Q: When do we want to use custom copy/move logic instead of the default generated ones? 
A: Custom logic should only be used if the class manages dynamic heap memory.

Q: When do we ever need to define an explicit move assignment instead of a shared one (rule of 4.5)
A: When we want the type to be move-only, we need to disable copy assignment. 

## Program Outputs And Explanation

Two resources are declared to showcase different copy/move mechanics. `r1` starts off with a length of 10 and has integers 0->9. More details are provided in the source code.

### Constructor outputs
**copy_constructor.cpp output**
```
Resource constructor // Resource r1 initialized with 0-9
Resource copy constructor // Resource r2 = r1;
Resource data: 0 1 2 3 4 5 6 7 8 9 // r1 data 
Resource data: 0 1 2 3 4 5 6 7 8 9 // r2 data
Resource destructor // r1 destructor
Resource destructor // r2 destructor
```
pretty self explanatory, calling `Resource r2 = r1;` calls the copy constructor instead of the default constructor, which deep copies the contents of `r1` into `r2`.
 
**move_constructor.cpp output:**
```
Resource constructor 
Resource move constructor // Resource r2 = std::move(r1);
Resource data: // r1 left in empty state
Resource data: 0 1 2 3 4 5 6 7 8 9 // r2 has r1's data
Resource destructor
Resource destructor
```
Similar to the copy constructor, but the move constructor causes `r2` to steal `r1`'s data. As shown in the output, `r1` is left in an empty state while `r2` has control of `r1`'s original data.

### Assignment outputs

**copy_assignment.cpp output:**
```
Resource constructor 
Resource default constructor // Resource r2; 
Resource copy constructor // temporary variable created
Resource assignment operator
Resource destructor // temporary variable destroyed
Resource data: 0 1 2 3 4 5 6 7 8 9 
Resource data: 0 1 2 3 4 5 6 7 8 9 
Resource destructor
Resource destructor
```
When we perform `r2 = r1`, a copy of `r1` is passed into the `operator=(Resource r)`, which is initialized with the copy constructor. when `operator=`completes, the copy is destroyed, resulting in a third destructor.

**move_assignment.cpp output:**
```
Resource constructor
Resource default constructor // Resource r2;
Resource move constructor // temporary variable created
Resource assignment operator
Resource destructor // temporary variable destroyed
Resource data: 
Resource data: 0 1 2 3 4 5 6 7 8 9 
Resource destructor
Resource destructor
```
Similar to the previous example but the temporary variable is created using the move constructor instead of the copy constructor, which is why `r2` steals `r1`'s data.

