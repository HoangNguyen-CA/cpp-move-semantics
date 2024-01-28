#ifndef RESOURCE_H
#define RESOURCE_H

#include <iostream>

// Resource holding an array of integers
class Resource
{
public:
    Resource();
    ~Resource();
    Resource(int size);
    Resource(const Resource &r);
    Resource(Resource &&r);

    Resource &operator=(Resource r);
    int &operator[](int index);
    void print();

private:
    int m_size{};
    int *m_data{};
    void swap(Resource &a, Resource &b);
};
#endif