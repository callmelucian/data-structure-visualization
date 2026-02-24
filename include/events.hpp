/**
 * This file includes UIEvent that will be used by all data structures,
 * here are the following events that are going to be triggered:
 * - CREATE: Create a node, given the shape (circle, rectangle), value
 *           and a boolen flag indicating whether should the UI annotate the value onto the node, return visualID
 * - DELETE: Delete a node and all links associated to it, given the visualID
 * - ADDLINK: Create an edge from a node to another, given the visualIDs
 *            and a boolean flag indicating whether the edge is directed/undirected
 * - REMOVELINK: Remove an edge from a node to another, given the visualIDs (throw an error if the edge doesn't exist)
 */

#include <string>

enum class EventType {
    CREATE,
    DELETE,
    ADDLINK,
    REMOVELINK,
};

struct UIEvent {
    EventType type;
    int idA, idB, value;
    bool flag;
    std::string shape;

    // constructor for CREATE event
    UIEvent (EventType type, int value, bool annotate, std::string shape) :
        type(type), idA(0), idB(0), value(value), flag(annotate), shape(shape) {}
    
    // constructor for ADDLINK event
    UIEvent (EventType type, int idA, int idB, bool flag) :
        type(type), idA(idA), idB(idB), value(0), flag(false), shape("") {}
};