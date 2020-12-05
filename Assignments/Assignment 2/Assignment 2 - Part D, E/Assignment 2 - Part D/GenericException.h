#pragma once
#include <string>
#include <exception>

using namespace std;


class DatabaseException :
    public exception
{
public:
    DatabaseException(const string& msg) : msg(msg) {}
    virtual const char* what() const throw() {
        return msg.c_str();
    }
private:
    string msg;
};


class SSCException :
    public exception
{
public:
    SSCException(const string& msg) : msg(msg) {}
    virtual const char* what() const throw() {
        return msg.c_str();
    }
private:
    string msg;
};

class StudentException :
    public exception
{
public:
    StudentException(const string& msg) : msg(msg) {}
    virtual const char* what() const throw() {
        return msg.c_str();
    }
private:
    string msg;
};
