#pragma once

#include <exception>
#include <sstream>
#include <vector>
#include <string>


/*
xml.hpp contains a basic XML parser. it parses the XML message into nodes and puts them into a vector.
nodes can be updated, and can contain different data types, but currently all the data is stored as
strings. there is also a type stored in the Node which can be used to determine the Type of the data
in Node::Value. This allows you to safely parse/cast the value in the node to the desired value for
use. 
*/

namespace xml {

struct ErrMalformedXML : std::exception {
    std::string err;
    ErrMalformedXML() : err{"error: malformed XML"} {}
    ErrMalformedXML(std::string s) : err{s} {}
    const char* what() const noexcept {return err.c_str();}
};

struct ErrEmptyMessage : std::exception {
    std::string err;
    ErrEmptyMessage() : err{"error: empty XML message"} {}
    ErrEmptyMessage(std::string s) : err{s} {}
    const char* what() const noexcept {return err.c_str();}
};

// all possible NodeTypes - these tell a user how to manage the
// data in the node.
enum NodeType {UINT, INT, FLOAT, STRING, PARENT};
NodeType StringToNodeType(std::string s);

struct Node {
    std::string name;
    std::string value;
    NodeType type;
    std::vector<Node> children;
    Node(std::string name, std::string value);
    Node(std::string name, std::vector<Node> children);
    std::string xml();
};

std::ostream& operator<<(std::ostream &os, Node &n);

struct XML {
    std::string raw_message;
    std::vector<Node> nodes;
    XML() = default;
    XML(const char* s);
    XML(const std::string s);
    void Parse();
    void Parse(const char* s);
    void Parse(std::string s);
    void clear();
    std::string Dump();
};

};