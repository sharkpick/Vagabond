#include "xml.hpp"
#include "string.hpp"

#include <iostream>

namespace xml {

NodeType StringToNodeType(std::string s) {
    if (string::is_uint(s)) {
        return UINT;
    } else if (string::is_integer(s)) {
        return INT;
    } else if (string::is_float(s)) {
        return FLOAT;
    } else {
        return STRING;
    }
}

std::string Node::xml() {
    std::string results = "<"+name+">";
    if (children.size() > 0) {
        for (auto child: children) {
            results.append(child.xml());
        }
    } else {
        results.append(value);
    }
    results.append("</"+name+">");
    return results;
}

Node::Node(std::string name, std::vector<Node> children) :
    name{name}, children{children}, type{PARENT} {}

Node::Node(std::string name, std::string value) :
    name{name}, value{value}, type{StringToNodeType(value)} {}

std::ostream& operator<<(std::ostream &os, Node &n) {
    os << n.xml();
    return os;
}

void XML::clear() {
    raw_message.clear();
    nodes.clear();
}

std::vector<Node> extract_nested_object(std::string s, size_t startIndex, std::string name) {
    std::vector<Node> results;
    for (size_t position = startIndex; position < s.length(); position++) {
        std::string startTag;
        std::string endTag;
        std::string body;
        auto c = s.at(position);
        if (c == '<') {
            for (position = position+1; position < s.length(); position++) {
                c = s.at(position);
                if (c == '>') break;
                startTag.push_back(c);
            }
            position = position + 1;
            if (position >= s.length()) {
                if (startTag.length() > 0 && startTag.front() == '/' && name == startTag.substr(1, startTag.length())) {
                    return results;
                } else {
                    std::cout << "extract_nested_object throwing error: "+startTag+"\n";
                    throw ErrMalformedXML("error: malformed XML "+s);
                }
            }
            c = s.at(position);
            if (c == '<') {
                auto children = extract_nested_object(s, position, startTag);
            } else {
                for (position = position; position < s.length(); position++) {
                    c = s.at(position);
                    if (c == '<') break;
                    body.push_back(c);
                }
                for (position = position + 1; position < s.length(); position++) {
                    c = s.at(position);
                    if (c == '>') break;
                    endTag.push_back(c);
                }
                if (startTag.length() > 0 && endTag.length() > 0 && endTag.front() == '/' && startTag == endTag.substr(1, endTag.length())) {
                    Node node(startTag, body);
                    results.push_back(node);
                } else {
                    std::cerr << "error: " << startTag << " " << body << " " << endTag << std::endl;
                    throw ErrMalformedXML("error: malformed XML "+s);
                }
                
                //std::cout << startTag << " " << body << " " << endTag << std::endl;
            }
        } else {
            std::cout << "throwing error from extracted_nested_object: " << s.substr(position, s.length()) << std::endl;
            throw ErrMalformedXML("error: extract_nested_object received something else... " + s);
        }
    }
    return results;
}

void XML::Parse(std::string s) {
    raw_message = s;
    for (size_t position = 0; position < s.length(); position++) {
        std::string startTag;
        std::string endTag;
        std::string body;
        auto c = s.at(position);
        if (c == '<') {
            for (position = position+1; position < s.length(); position++) {
                c = s.at(position);
                if (c == '>') break;
                startTag.push_back(c);
            }
            // here we could either have data in a cell or a nested object.
            position = position + 1;
            if (position >= s.length()) throw ErrMalformedXML("error: malformed XML "+s);
            c = s.at(position);
            if (c == '<') {
                auto children = extract_nested_object(s, position, startTag);
                Node node(startTag, children);
                nodes.push_back(node);
            } else {
                for (position = position; position < s.length(); position++) {
                    c = s.at(position);
                    if (c == '<') break;
                    body.push_back(c);
                }
                for (position = position+1; position < s.length(); position++) {
                    c = s.at(position);
                    if (c == '>') break;
                    endTag.push_back(c);
                }
                if (startTag.length() > 0 && endTag.length() > 0 && endTag.front() == '/' && startTag == endTag.substr(1, endTag.length())) {
                    Node node(startTag, body);
                    nodes.push_back(node);
                } else {
                    std::cerr << "error: " << startTag << " " << body << " " << endTag << std::endl;
                    throw ErrMalformedXML("error: malformed XML "+s);
                }
            }       
        } else {
            throw ErrMalformedXML("error: malformed XML missing leading '<' "+ s);
        }
    }
}

void XML::Parse(const char* s) {
    raw_message = std::string(s);
    Parse(raw_message);
}

void XML::Parse() {
    if (raw_message.length() == 0) {
        throw ErrEmptyMessage();
    }
    Parse(raw_message);
}

XML::XML(const char* s) : raw_message{std::string(s)} {
    Parse(raw_message);
}

XML::XML(std::string s) : raw_message{std::string(s)} {
    Parse(raw_message);
}

std::string XML::Dump() {
    std::string results;
    for (auto &e: nodes) {
        results.append(e.xml());
    }
    return results;
}

};