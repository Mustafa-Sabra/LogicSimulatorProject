#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

/**********************************************************************************/
class Node{
    friend std::ostream& operator<<(std::ostream& out, const Node& node);
private:
    string name;
    bool value;
public:
    Node(){}        //default ctor
    Node(const string& str):       //non-default ctor
                    name(str){}        
    string getName(void);
    bool getValue(void);
    void setName(const string& str);
    void setValue(const bool& number);
    
    bool AND(const Node& node);
    bool OR(const Node& node);
    bool XOR(const Node& node);
    bool NOT(void);

};

std::ostream& operator<<(std::ostream& out, const Node& node)
{
    out << node.name <<" "<< node.value;
    return out;
}
string Node::getName(void)
    {
        return name;
    }
bool Node::getValue(void)
    {
        return value;
    }
void Node::setName(const string& str)
    {
        name = str;
    }
void Node::setValue(const bool& number)
    {
        value = number;
    }

bool Node::AND(const Node& node)
{
    return (value & node.value);
}
bool Node::OR(const Node& node)
{
    return (value | node.value);
}
bool Node::XOR(const Node& node)
{
    return (value ^ node.value);
}
bool Node::NOT(void){
    return !value;
}
/*********************************************************************************/

/**********************************************************************************/
class Gate{
private:
    string gateType;
    Node *inputNode_1;
    Node *inputNode_2;
    Node *outputNode;
public:
    Gate(){}
    Gate(const string& gate ):gateType(gate){}

    string getType(void);
    Node* getInputNode_1(void);
    Node* getInputNode_2(void);
    Node* getOutputNode(void);
    void setType(const string& str);
    void setInputNode_1(Node *node_1);
    void setInputNode_2(Node * node_2);
    void setOutputNode(Node * node_3);
    bool simulateGate();
};
string Gate::getType(void){
    return gateType;
}
Node* Gate::getInputNode_1(void){
    return inputNode_1;
}
Node* Gate::getInputNode_2(void){
    return inputNode_2;
}
Node* Gate::getOutputNode(void){
    return outputNode;
}

void Gate::setType(const string& str){
    gateType = str;
}
void Gate::setInputNode_1(Node * node_1){
    inputNode_1 = node_1;
}
void Gate::setInputNode_2(Node * node_2){
    inputNode_2 = node_2;
}
void Gate::setOutputNode(Node * node_3){
    outputNode = node_3;
}

bool Gate::simulateGate(){
    bool result=0;
    if("AND" == gateType)
    {
        result =  inputNode_1->AND(*inputNode_2);
    }
    else if("OR" == gateType)
    {
        result =  inputNode_1->OR(*inputNode_2);
    }
    else if("NAND" == gateType)
    {
        result =  !(inputNode_1->AND(*inputNode_2));
    }
    else if("NOR" == gateType)
    {
        result =  !(inputNode_1->OR(*inputNode_2));
    }
    else if("XOR" == gateType)
    {
        result =  inputNode_1->XOR(*inputNode_2);
    }
    else if("XNOR" == gateType)
    {
        result =  !(inputNode_1->XOR(*inputNode_2));
    }
    else if("NOT" == gateType)
    {
        result =  inputNode_1->NOT();
    }
    else
    {
        cout << "Wrong Gate Name ...!\n";
    }
    return result;
}
/**********************************************************************************/

/**********************************************************************************/
class Simulator{
    friend Simulator& create_simulator(void);
private:
    std::vector <Gate*> gateArr;
    std::vector <Node*> nodeArr;
    Simulator(){};
public:
    Simulator(const Simulator& object) = delete;
    void postGate(Gate * gatePtr);
    void postNode(Node * nodePtr);
    Node* findNode(const string& nodeName);
    void getNodes();
    void deallocateGates(void);
    void deallocateNode(void);
    void startSimulate();
};
void Simulator::deallocateGates(void){
    for(Gate* ptr: gateArr){
        delete ptr;
    }
}
void Simulator::deallocateNode(void){
    for(Node * ptr : nodeArr){
        delete ptr;
    }
}
void Simulator::getNodes(){
    for( Node* nodePtr: nodeArr)
    {
        cout << nodePtr->getName() << ": "<< nodePtr->getValue() <<endl;

    }
}
void Simulator::postGate(Gate * gatePtr){
    gateArr.push_back(gatePtr);
}
void Simulator::postNode(Node * nodePtr){
    nodeArr.push_back(nodePtr);
}
Node* Simulator::findNode(const string& nodeName){
    Node * temp = nullptr; 
    for(Node* nodePtr: nodeArr){
        if(nodePtr->getName() == nodeName){
            temp = nodePtr;
            break;
        }
    }
    return temp;
}

void Simulator::startSimulate()
{
    for ( Gate*  gateAddress : gateArr) {
        gateAddress->getOutputNode()->setValue(gateAddress->simulateGate());
    }
}
Simulator& create_simulator(void){
    static Simulator object;

    return object;
}
/**********************************************************************************/

/**********************************************************************************/
class GateGenerator{
    Node* createNode(const string& node_name);
    Gate* createGate(const string& str);
public:
    void parseInput();
};
void GateGenerator::parseInput(){
    Gate * gatePtr;
    Node * nodePtr;
    Simulator& simulator = create_simulator();
    string str;
    string inputNode_1;
    string inputNode_2;
    string outputNode;
    string nodeName;
    bool nodeValue;
    while(1){
        cin >> str;
        if(str == "AND" || str == "OR" || 
            str == "XOR" || str == "NAND" ||
            str == "NOR" || str == "XNOR" 
            )
        {
            gatePtr = createGate(str);

            cin >> inputNode_1 >> inputNode_2 >> outputNode;
            nodePtr = simulator.findNode(inputNode_1);
            if(nodePtr == nullptr){
                nodePtr = createNode(inputNode_1);
                simulator.postNode(nodePtr);
            }
            gatePtr->setInputNode_1(nodePtr);

            nodePtr = simulator.findNode(inputNode_2);
            if(nodePtr == nullptr){
                nodePtr = createNode(inputNode_2);
                simulator.postNode(nodePtr);

            }
            gatePtr->setInputNode_2(nodePtr);

            nodePtr = simulator.findNode(outputNode);
            if(nodePtr == nullptr){
                nodePtr = createNode(outputNode);
                simulator.postNode(nodePtr);

            }
            gatePtr->setOutputNode(nodePtr);

            simulator.postGate(gatePtr);
        }
        else if(str == "NOT"){
            gatePtr = createGate(str);
            cin >> inputNode_1 >> outputNode;

            nodePtr = simulator.findNode(inputNode_1);
            if(nodePtr == nullptr){
                nodePtr = createNode(inputNode_1);
                simulator.postNode(nodePtr);
            }
            gatePtr->setInputNode_1(nodePtr);

            nodePtr = simulator.findNode(outputNode);
            if(nodePtr == nullptr){
                nodePtr = createNode(outputNode);
                simulator.postNode(nodePtr);

            }
            gatePtr->setOutputNode(nodePtr);

            simulator.postGate(gatePtr);
        }
        else if(str == "SET")
        {
            cin >>  nodeName >> nodeValue;
            nodePtr = simulator.findNode(nodeName);
            nodePtr->setValue(nodeValue);
        }
        else if(str == "SIM")
        {
            simulator.startSimulate();
        }
        else if(str == "OUT")
        {
            cin >> outputNode;
            if(outputNode == "ALL"){
                simulator.getNodes();
                break;
            }else{
                nodePtr = simulator.findNode(outputNode);
                cout << nodePtr->getName() << ": "<<nodePtr->getValue()<<endl;
            }
        }
        
    }

    simulator.deallocateNode();
    simulator.deallocateGates();
}
Node* GateGenerator::createNode(const string& node_name){
    Node *node = new Node(node_name);
    return node;
}
Gate* GateGenerator::createGate(const string& str){
    Gate *gate = new Gate(str);

    return gate;
}
/**********************************************************************************/
int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */   
    
    GateGenerator gate;
    gate.parseInput();
    return 0;
}